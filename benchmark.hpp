// КОД НЕ МОЙ!!!

// benchmark.hpp — замер времени и ресурсов потока (C++17, header-only)
//
// ИСПОЛЬЗОВАНИЕ
//
//     #include "benchmark.hpp"
//
//     int main() {
//         bm::test("sum", [] {
//             std::uint64_t s = 0;
//             for (int i = 0; i < 100'000'000; ++i) s += i;
//             bm::keep(s);                       // не даём оптимизатору выкинуть работу
//         });
//
//         bm::Report r = bm::run([] { /* ... */ });
//         std::cout << r.wall_s() << " s\n";
//
//         auto st = bm::repeat(10, [] { /* ... */ });   // статистика по прогонам
//         std::cout << st;
//     }
//
// СБОРКА
//
//     Linux/macOS  g++ -std=c++17 -O2 -pthread main.cpp
//     MinGW        g++ -std=c++17 -O2 main.cpp -lpsapi
//     MSVC         cl /std:c++17 /O2 /EHsc main.cpp
//     Emscripten   em++ -std=c++17 -O2 -pthread main.cpp   (без -pthread тест идёт в главном потоке)
//
// МЕТРИКИ ПО СИСТЕМАМ
//
//     Метрика              Linux  macOS  Windows  Emscripten
//     Wall time              +      +       +         +
//     CPU time               +      +       +         -
//     User / System          +      +       +         -
//     CPU cycles             -      -       +         -
//     Context switches       +      -       -         -
//     Page faults            +      -       -         -
//     Peak memory (процесс)  +      +       +         +
//
// ПРИМЕЧАНИЯ
//     - Peak memory относится ко всему процессу, не к потоку.
//     - Windows: _WIN32_WINNT >= 0x0600 для QueryThreadCycleTime.

#pragma once

#if defined(__linux__) && !defined(_GNU_SOURCE)
	#define _GNU_SOURCE   // для RUSAGE_THREAD; должно быть до системных заголовков
#endif

#include <algorithm>
#include <cmath>
#include <memory>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
#include <iostream>

#if defined(_WIN32)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	#include <windows.h>
	#include <psapi.h>
	#if defined(_MSC_VER)
		#pragma comment(lib, "psapi.lib")
	#endif
#elif defined(__EMSCRIPTEN__)
	#include <emscripten.h>
	#include <emscripten/heap.h>
#else
	#include <time.h>
	#include <sys/resource.h>
	#include <sys/time.h>
	#if defined(__APPLE__)
		#include <mach/mach.h>
	#endif
#endif

namespace bm {

// ------------------------------------------------------------------
// Флаги доступных метрик
// ------------------------------------------------------------------

	enum Flag : unsigned {
		HasCpu     = 1u << 0,  // суммарное CPU-время потока
		HasSplit   = 1u << 1,  // раздельно user / system
		HasCycles  = 1u << 2,  // такты CPU потока
		HasCtx     = 1u << 3,  // переключения контекста
		HasFaults  = 1u << 4,  // page faults
		HasPeakMem = 1u << 5   // пиковая память (весь процесс)
	};

// ------------------------------------------------------------------
// Результат одного замера
// ------------------------------------------------------------------

	struct Report {
		std::uint64_t wall_ns = 0;
		std::uint64_t cpu_ns = 0;
		std::uint64_t user_ns = 0;
		std::uint64_t sys_ns = 0;
		std::uint64_t cycles = 0;
		std::uint64_t vol_ctx = 0;
		std::uint64_t invol_ctx = 0;
		std::uint64_t minor_faults = 0;
		std::uint64_t major_faults = 0;
		std::uint64_t peak_mem_bytes = 0;   // пик процесса после теста
		unsigned flags = 0;

		bool has(Flag f) const noexcept {
			return (flags & f) != 0;
		}

		double wall_s() const noexcept {
			return static_cast<double>(wall_ns) * 1e-9;
		}
		double wall_ms() const noexcept {
			return static_cast<double>(wall_ns) * 1e-6;
		}
		double cpu_s() const noexcept {
			return static_cast<double>(cpu_ns) * 1e-9;
		}

		// Доля CPU от wall-времени, в процентах (0, если CPU-время недоступно)
		double cpu_utilization() const noexcept {
			return (has(HasCpu) && wall_ns) ? 100.0 * double(cpu_ns) / double(wall_ns) : 0.0;
		}
		double peak_mem_mib() const noexcept {
			return double(peak_mem_bytes) / (1024.0 * 1024.0);
		}

		std::string str() const;
	};

// ------------------------------------------------------------------
// Не даём компилятору удалить результат вычислений
// ------------------------------------------------------------------

	template <class T>
	inline void keep(T&& v) noexcept {
		#if defined(__GNUC__) || defined(__clang__)
			asm volatile ("" : : "r,m" (v) : "memory");
		#else
			static volatile char sink;
			sink = *reinterpret_cast<const volatile char*>(std::addressof(v));
		#endif
	}

	inline void clobber() noexcept {
		#if defined(__GNUC__) || defined(__clang__)
			asm volatile ("" : : : "memory");
		#endif
	}

	namespace detail {

// ------------------------------------------------------------------
// Время
// ------------------------------------------------------------------

		inline std::uint64_t wall_now_ns() noexcept {
			using namespace std::chrono;
			return static_cast<std::uint64_t>(
				duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count());
		}

		#if defined(_WIN32)
			inline std::uint64_t ft_ns(const FILETIME& f) noexcept {
				ULARGE_INTEGER u;
				u.LowPart = f.dwLowDateTime;
				u.HighPart = f.dwHighDateTime;
				return u.QuadPart * 100ULL;   // единицы по 100 нс
			}
		#elif !defined(__EMSCRIPTEN__)
			inline std::uint64_t tv_ns(const timeval& tv) noexcept {
				return std::uint64_t(tv.tv_sec) * 1000000000ULL + std::uint64_t(tv.tv_usec) * 1000ULL;
			}
		#endif

// ------------------------------------------------------------------
// Снимок ресурсов текущего потока
// ------------------------------------------------------------------

		struct Snap {
			std::uint64_t cpu_ns = 0, user_ns = 0, sys_ns = 0, cycles = 0;
			std::uint64_t vol_ctx = 0, invol_ctx = 0, minor_faults = 0, major_faults = 0;
			unsigned flags = 0;
		};

		inline Snap snapshot() noexcept {
			Snap s;

			#if defined(_WIN32)

				FILETIME c, e, k, u;
				if (GetThreadTimes(GetCurrentThread(), &c, &e, &k, &u)) {
					s.sys_ns = ft_ns(k);
					s.user_ns = ft_ns(u);
					s.cpu_ns = s.sys_ns + s.user_ns;
					s.flags |= HasCpu | HasSplit;
				}
				ULONG64 cyc = 0;
				if (QueryThreadCycleTime(GetCurrentThread(), &cyc)) {
					s.cycles = cyc;
					s.flags |= HasCycles;
				}

			#elif defined(__EMSCRIPTEN__)

				// CPU-метрик потока нет

			#else

				#if defined(CLOCK_THREAD_CPUTIME_ID)
					{
						timespec ts;
						if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts) == 0) {
							s.cpu_ns = std::uint64_t(ts.tv_sec) * 1000000000ULL + std::uint64_t(ts.tv_nsec);
							if (s.cpu_ns) s.flags |= HasCpu;
						}
					}
				#endif

				#if defined(RUSAGE_THREAD)   // Linux, FreeBSD
					{
						rusage ru;
						if (getrusage(RUSAGE_THREAD, &ru) == 0) {
							s.user_ns = tv_ns(ru.ru_utime);
							s.sys_ns = tv_ns(ru.ru_stime);
							s.vol_ctx = std::uint64_t(ru.ru_nvcsw);
							s.invol_ctx = std::uint64_t(ru.ru_nivcsw);
							s.minor_faults = std::uint64_t(ru.ru_minflt);
							s.major_faults = std::uint64_t(ru.ru_majflt);
							s.flags |= HasSplit | HasCtx | HasFaults;
						}
					}
				#elif defined(__APPLE__)
					{
						thread_basic_info_data_t info;
						mach_msg_type_number_t count = THREAD_BASIC_INFO_COUNT;
						mach_port_t port = mach_thread_self();
						kern_return_t kr = thread_info(port, THREAD_BASIC_INFO, (thread_info_t)&info, &count);
						mach_port_deallocate(mach_task_self(), port);

						if (kr == KERN_SUCCESS) {
							s.user_ns = std::uint64_t(info.user_time.seconds) * 1000000000ULL
							    + std::uint64_t(info.user_time.microseconds) * 1000ULL;
							s.sys_ns = std::uint64_t(info.system_time.seconds) * 1000000000ULL
							    + std::uint64_t(info.system_time.microseconds) * 1000ULL;
							s.flags |= HasSplit;
							if (!(s.flags & HasCpu)) {
								s.cpu_ns = s.user_ns + s.sys_ns;
								s.flags |= HasCpu;
							}
						}
					}
				#endif

			#endif
			return s;
		}

// Пиковая память ПРОЦЕССА (по-потоковой ОС не дают)
		inline std::uint64_t peak_mem() noexcept {
			#if defined(_WIN32)
				PROCESS_MEMORY_COUNTERS pmc;
				pmc.cb = sizeof pmc;
				if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof pmc)) return std::uint64_t(pmc.PeakWorkingSetSize);
				return 0;
			#elif defined(__EMSCRIPTEN__)
				return std::uint64_t(emscripten_get_heap_size());
			#else
				rusage ru;
				if (getrusage(RUSAGE_SELF, &ru) != 0) return 0;
				#if defined(__APPLE__)
					return std::uint64_t(ru.ru_maxrss);          // байты
				#else
					return std::uint64_t(ru.ru_maxrss) * 1024ULL; // КБ
				#endif
			#endif
		}

// Тело замера: выполняется в целевом потоке
		template <class F>
		inline void measure(F& f, Report& r) {
			const Snap a = snapshot();
			const std::uint64_t t0 = wall_now_ns();
			clobber();

			f();

			clobber();
			const std::uint64_t t1 = wall_now_ns();
			const Snap b = snapshot();

			r.wall_ns = t1 - t0;
			r.flags = a.flags & b.flags;
			r.cpu_ns = b.cpu_ns - a.cpu_ns;
			r.user_ns = b.user_ns - a.user_ns;
			r.sys_ns = b.sys_ns - a.sys_ns;
			r.cycles = b.cycles - a.cycles;
			r.vol_ctx = b.vol_ctx - a.vol_ctx;
			r.invol_ctx = b.invol_ctx - a.invol_ctx;
			r.minor_faults = b.minor_faults - a.minor_faults;
			r.major_faults = b.major_faults - a.major_faults;
			r.peak_mem_bytes = peak_mem();
			if (r.peak_mem_bytes) r.flags |= HasPeakMem;
		}

// Обёртка: возвращаемое значение функции гасим через keep()
		template <class F>
		struct Invoker {
			F& f;
			void operator()() {
				if constexpr (std::is_void_v<std::invoke_result_t<F&> >) {
					f();
				}
				else {
					auto&& v = f();
					keep(v);
				}
			}
		};

	} // namespace detail

// ------------------------------------------------------------------
// Основной API
// ------------------------------------------------------------------

// Запускает callable в отдельном потоке и возвращает Report.
// Исключение из callable пробрасывается в вызывающий поток.
// Если потоки недоступны (Emscripten без -pthread), тест идёт в текущем.
	template <class F>
	Report run(F&& func) {
		Report r;
		detail::Invoker<std::remove_reference_t<F> > inv{ func };

		#if defined(__EMSCRIPTEN__) && !defined(__EMSCRIPTEN_PTHREADS__)
			detail::measure(inv, r);
		#else
			std::exception_ptr err;
			std::thread th([&] {
			    try { detail::measure(inv, r); }
			    catch (...) { err = std::current_exception(); }
			});
			th.join();
			if (err) std::rethrow_exception(err);
		#endif
		return r;
	}

// Прогрев + n замеров. Возвращает все отчёты.
	template <class F>
	std::vector<Report> run_n(std::size_t n, F&& func, std::size_t warmup = 1) {
		for (std::size_t i = 0; i < warmup; ++i) (void)run(func);
		std::vector<Report> v;
		v.reserve(n);
		for (std::size_t i = 0; i < n; ++i) v.push_back(run(func));
		return v;
	}

// Статистика по wall-времени серии прогонов
	struct Stats {
		std::size_t runs = 0;
		std::uint64_t min_ns = 0, max_ns = 0, median_ns = 0;
		double mean_ns = 0, stddev_ns = 0;
		Report best;   // отчёт самого быстрого прогона
	};

	inline Stats summarize(const std::vector<Report>& v) {
		Stats s;
		if (v.empty()) return s;
		s.runs = v.size();

		std::vector<std::uint64_t> w;
		w.reserve(v.size());
		double sum = 0;
		for (const auto& r : v) {
			w.push_back(r.wall_ns); sum += double(r.wall_ns);
		}
		std::sort(w.begin(), w.end());

		s.min_ns = w.front();
		s.max_ns = w.back();
		s.median_ns = (w.size() % 2) ? w[w.size() / 2]
		                                     : (w[w.size() / 2 - 1] + w[w.size() / 2]) / 2;
		s.mean_ns = sum / double(w.size());

		double var = 0;
		for (auto x : w) {
			double d = double(x) - s.mean_ns; var += d * d;
		}
		s.stddev_ns = std::sqrt(var / double(w.size()));

		s.best = *std::min_element(v.begin(), v.end(),
		        [](const Report& a, const Report& b) {
			return a.wall_ns < b.wall_ns;
		});
		return s;
	}

	template <class F>
	Stats repeat(std::size_t n, F&& func, std::size_t warmup = 1) {
		return summarize(run_n(n, std::forward<F>(func), warmup));
	}

// ------------------------------------------------------------------
// Вывод
// ------------------------------------------------------------------

	inline std::ostream& operator<<(std::ostream& os, const Report& r) {
		auto sec = [](std::uint64_t ns) {
				return double(ns) * 1e-9;
			};
		const auto old_flags = os.flags();
		const auto old_prec = os.precision();
		os << std::fixed;

		os << "Wall time:        " << r.wall_ns << " ns  (" << std::setprecision(6) << sec(r.wall_ns) << " s)\n";

		if (r.has(HasCpu)) os << "CPU time:         " << r.cpu_ns << " ns  (" << std::setprecision(6) << sec(r.cpu_ns) << " s)\n";
		else os << "CPU time:         n/a\n";

		if (r.has(HasSplit)) {
			os << "  user:           " << r.user_ns << " ns  (" << std::setprecision(6) << sec(r.user_ns) << " s)\n";
			os << "  system:         " << r.sys_ns << " ns  (" << std::setprecision(6) << sec(r.sys_ns) << " s)\n";
		}
		if (r.has(HasCycles)) os << "CPU cycles:       " << r.cycles << "  (" << std::setprecision(3) << double(r.cycles) / 1e6 << " M)\n";

		if (r.has(HasCpu) && r.wall_ns) os << "CPU utilization:  " << std::setprecision(1) << r.cpu_utilization() << " %\n";

		if (r.has(HasCtx)) os << "Ctx switches:     " << r.vol_ctx << " voluntary, " << r.invol_ctx << " involuntary\n";
		if (r.has(HasFaults)) os << "Page faults:      " << r.minor_faults << " minor, " << r.major_faults << " major\n";
		if (r.has(HasPeakMem)) os << "Peak memory:      " << r.peak_mem_bytes << " B  (" << std::setprecision(2)
			                      << r.peak_mem_mib() << " MiB, whole process)\n";

		os.flags(old_flags);
		os.precision(old_prec);
		return os;
	}

	inline std::string Report::str() const {
		std::ostringstream ss;
		ss << *this;
		return ss.str();
	}

	inline std::ostream& operator<<(std::ostream& os, const Stats& s) {
		const auto old_flags = os.flags();
		const auto old_prec = os.precision();
		os << std::fixed << std::setprecision(3);
		os << "Runs:    " << s.runs << '\n'
		   << "Min:     " << double(s.min_ns) * 1e-6 << " ms\n"
		   << "Median:  " << double(s.median_ns) * 1e-6 << " ms\n"
		   << "Mean:    " << s.mean_ns * 1e-6 << " ms  (± " << s.stddev_ns * 1e-6 << ")\n"
		   << "Max:     " << double(s.max_ns) * 1e-6 << " ms\n";
		os.flags(old_flags);
		os.precision(old_prec);
		return os;
	}

// Запустить, напечатать результат (опционально с именем), вернуть Report
	template <class F>
	Report test(std::string_view name, F&& func, std::ostream& os = std::cout) {
		Report r = run(std::forward<F>(func));
		if (!name.empty()) os << "=== " << name << " ===\n";
		os << r;
		return r;
	}

	template <class F, class = std::enable_if_t<std::is_invocable_v<F&> > >
	Report test(F&& func, std::ostream& os = std::cout) {
		return test(std::string_view{}, std::forward<F>(func), os);
	}

} // namespace bm