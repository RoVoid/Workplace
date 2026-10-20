// Not my code!!!

#ifndef BENCHMARK_H
#define BENCHMARK_H

/* Для RUSAGE_THREAD на Linux. Должно быть определено до любых системных заголовков. */
#if defined(__linux__) && !defined(_GNU_SOURCE)
	#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

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
		#pragma comment(lib, "psapi.lib")   /* MinGW: -lpsapi */
	#endif

#elif defined(__EMSCRIPTEN__)

	#include <time.h>
	#include <emscripten.h>
	#include <emscripten/heap.h>

	#if defined(__EMSCRIPTEN_PTHREADS__)
		#include <pthread.h>
	#endif

#else

	#include <time.h>
	#include <pthread.h>
	#include <sys/time.h>
	#include <sys/resource.h>

	#if defined(__APPLE__)
		#include <mach/mach.h>
	#endif

#endif


/* ------------------------------------------------------------------ */
/* Типы                                                               */
/* ------------------------------------------------------------------ */

typedef int (*test_func)(void);

/* Какие метрики реально доступны на текущей платформе */
enum {
	BENCH_HAS_CPU      = 1 << 0,  /* суммарное CPU-время потока           */
	BENCH_HAS_SPLIT    = 1 << 1,  /* раздельно user / system              */
	BENCH_HAS_CYCLES   = 1 << 2,  /* такты CPU потока                     */
	BENCH_HAS_CTX      = 1 << 3,  /* переключения контекста               */
	BENCH_HAS_FAULTS   = 1 << 4,  /* page faults                          */
	BENCH_HAS_PEAK_MEM = 1 << 5   /* пиковая память (на весь процесс)     */
};

typedef struct {
	uint64_t cpu_ns;
	uint64_t user_ns;
	uint64_t sys_ns;
	uint64_t cycles;
	uint64_t vol_ctx;      /* добровольные переключения   */
	uint64_t invol_ctx;    /* принудительные переключения */
	uint64_t minor_faults;
	uint64_t major_faults;
	unsigned flags;
} bench_snap;

typedef struct {
	uint64_t wall_ns;
	uint64_t cpu_ns;
	uint64_t user_ns;
	uint64_t sys_ns;
	uint64_t cycles;
	uint64_t vol_ctx;
	uint64_t invol_ctx;
	uint64_t minor_faults;
	uint64_t major_faults;
	uint64_t peak_mem_bytes;   /* пик процесса после теста */
	unsigned flags;
	int value;           /* значение, которое вернула функция */
} bench_result;


/* ------------------------------------------------------------------ */
/* Время                                                              */
/* ------------------------------------------------------------------ */

/* Монотонное время в наносекундах. */
static inline uint64_t bm_get_ns(void) {
	#if defined(_WIN32)

		static LARGE_INTEGER frequency = { 0 };
		LARGE_INTEGER counter;

		if (frequency.QuadPart == 0) QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&counter);

		/* Без переполнения: отдельно секунды и остаток */
		uint64_t freq = (uint64_t)frequency.QuadPart;
		uint64_t cnt = (uint64_t)counter.QuadPart;

		return (cnt / freq) * 1000000000ULL
		       + (cnt % freq) * 1000000000ULL / freq;

	#elif defined(__EMSCRIPTEN__)

		return (uint64_t)(emscripten_get_now() * 1000000.0);

	#else

		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (uint64_t)ts.tv_sec * 1000000000ULL
		       + (uint64_t)ts.tv_nsec;

	#endif
}


/* CPU-время текущего потока в наносекундах (0 и без флага, если недоступно). */
static inline uint64_t bm_get_thread_cpu_ns(void) {
	#if defined(_WIN32)

		FILETIME c, e, k, u;
		ULARGE_INTEGER kt, ut;

		if (!GetThreadTimes(GetCurrentThread(), &c, &e, &k, &u)) return 0;

		kt.LowPart = k.dwLowDateTime; kt.HighPart = k.dwHighDateTime;
		ut.LowPart = u.dwLowDateTime; ut.HighPart = u.dwHighDateTime;

		return (kt.QuadPart + ut.QuadPart) * 100ULL;   /* единицы по 100 нс */

	#elif defined(CLOCK_THREAD_CPUTIME_ID) && !defined(__EMSCRIPTEN__)

		struct timespec ts;

		if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts) != 0) return 0;

		return (uint64_t)ts.tv_sec * 1000000000ULL
		       + (uint64_t)ts.tv_nsec;

	#else

		return 0;   /* Emscripten и прочее: честно говорим «недоступно» */

	#endif
}


/* ------------------------------------------------------------------ */
/* Ресурсы потока                                                     */
/* ------------------------------------------------------------------ */

#if !defined(_WIN32)
	static inline uint64_t bm_bench_tv_ns(struct timeval tv) {
		return (uint64_t)tv.tv_sec * 1000000000ULL
		       + (uint64_t)tv.tv_usec * 1000ULL;
	}
#endif

/* Снимок ресурсов текущего потока. */
static inline void bm_bench_snapshot(bench_snap *s) {
	memset(s, 0, sizeof *s);

	s->cpu_ns = bm_get_thread_cpu_ns();
	if (s->cpu_ns) s->flags |= BENCH_HAS_CPU;

	#if defined(_WIN32)

		{
			FILETIME c, e, k, u;
			if (GetThreadTimes(GetCurrentThread(), &c, &e, &k, &u)) {
				ULARGE_INTEGER kt, ut;
				kt.LowPart = k.dwLowDateTime; kt.HighPart = k.dwHighDateTime;
				ut.LowPart = u.dwLowDateTime; ut.HighPart = u.dwHighDateTime;
				s->sys_ns = kt.QuadPart * 100ULL;
				s->user_ns = ut.QuadPart * 100ULL;
				s->flags |= BENCH_HAS_CPU | BENCH_HAS_SPLIT;
			}
		}
		{
			ULONG64 cycles = 0;
			if (QueryThreadCycleTime(GetCurrentThread(), &cycles)) {
				s->cycles = (uint64_t)cycles;
				s->flags |= BENCH_HAS_CYCLES;
			}
		}

	#elif defined(RUSAGE_THREAD)    /* Linux, FreeBSD */

		{
			struct rusage ru;
			if (getrusage(RUSAGE_THREAD, &ru) == 0) {
				s->user_ns = bm_bench_tv_ns(ru.ru_utime);
				s->sys_ns = bm_bench_tv_ns(ru.ru_stime);
				s->vol_ctx = (uint64_t)ru.ru_nvcsw;
				s->invol_ctx = (uint64_t)ru.ru_nivcsw;
				s->minor_faults = (uint64_t)ru.ru_minflt;
				s->major_faults = (uint64_t)ru.ru_majflt;
				s->flags |= BENCH_HAS_SPLIT | BENCH_HAS_CTX | BENCH_HAS_FAULTS;
			}
		}

	#elif defined(__APPLE__)

		{
			thread_basic_info_data_t info;
			mach_msg_type_number_t count = THREAD_BASIC_INFO_COUNT;
			mach_port_t port = mach_thread_self();
			kern_return_t kr = thread_info(
				port, THREAD_BASIC_INFO, (thread_info_t)&info, &count
			);
			mach_port_deallocate(mach_task_self(), port);

			if (kr == KERN_SUCCESS) {
				s->user_ns = (uint64_t)info.user_time.seconds * 1000000000ULL
				    + (uint64_t)info.user_time.microseconds * 1000ULL;
				s->sys_ns = (uint64_t)info.system_time.seconds * 1000000000ULL
				    + (uint64_t)info.system_time.microseconds * 1000ULL;
				s->flags |= BENCH_HAS_SPLIT;

				if (!(s->flags & BENCH_HAS_CPU)) {
					s->cpu_ns = s->user_ns + s->sys_ns;
					s->flags |= BENCH_HAS_CPU;
				}
			}
		}

	#endif
}


/* Пиковая память ПРОЦЕССА (по-потоковой пиковой памяти ОС не дают). */
static inline uint64_t bm_bench_peak_mem(void) {
	#if defined(_WIN32)

		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof pmc)) return (uint64_t)pmc.PeakWorkingSetSize;
		return 0;

	#elif defined(__EMSCRIPTEN__)

		return (uint64_t)emscripten_get_heap_size();

	#else

		struct rusage ru;
		if (getrusage(RUSAGE_SELF, &ru) != 0) return 0;

		#if defined(__APPLE__)
			return (uint64_t)ru.ru_maxrss;           /* байты */
		#else
			return (uint64_t)ru.ru_maxrss * 1024ULL; /* КБ */
		#endif

	#endif
}


/* ------------------------------------------------------------------ */
/* Запуск в отдельном потоке                                          */
/* ------------------------------------------------------------------ */

typedef struct {
	test_func func;
	bench_result res;
} bench_context;

static inline void bm_bench_body(bench_context *ctx) {
	bench_snap a, b;
	uint64_t wall_start, wall_end;

	bm_bench_snapshot(&a);
	wall_start = bm_get_ns();

	ctx->res.value = ctx->func();

	wall_end = bm_get_ns();
	bm_bench_snapshot(&b);

	bench_result *r = &ctx->res;

	r->wall_ns = wall_end - wall_start;
	r->flags = a.flags & b.flags;
	r->cpu_ns = b.cpu_ns         - a.cpu_ns;
	r->user_ns = b.user_ns        - a.user_ns;
	r->sys_ns = b.sys_ns         - a.sys_ns;
	r->cycles = b.cycles         - a.cycles;
	r->vol_ctx = b.vol_ctx        - a.vol_ctx;
	r->invol_ctx = b.invol_ctx      - a.invol_ctx;
	r->minor_faults = b.minor_faults   - a.minor_faults;
	r->major_faults = b.major_faults   - a.major_faults;
	r->peak_mem_bytes = bm_bench_peak_mem();

	if (r->peak_mem_bytes) r->flags |= BENCH_HAS_PEAK_MEM;
}

#if defined(_WIN32)

	static DWORD WINAPI bm_bench_thread_entry(LPVOID arg) {
		bm_bench_body((bench_context *)arg);
		return 0;
	}

#elif !defined(__EMSCRIPTEN__) || defined(__EMSCRIPTEN_PTHREADS__)

	static void *bm_bench_thread_entry(void *arg) {
		bm_bench_body((bench_context *)arg);
		return NULL;
	}

#endif


/*
 * Запускает func в отдельном потоке и заполняет result.
 * Возвращает 0 при успехе.
 */
static inline int bm_bench_run(test_func func, bench_result *result) {
	bench_context ctx;
	memset(&ctx, 0, sizeof ctx);
	ctx.func = func;

	#if defined(_WIN32)

		HANDLE h = CreateThread(NULL, 0, bm_bench_thread_entry, &ctx, 0, NULL);
		if (!h) return -1;

		WaitForSingleObject(h, INFINITE);
		CloseHandle(h);

	#elif defined(__EMSCRIPTEN__) && !defined(__EMSCRIPTEN_PTHREADS__)

		/* Потоков нет (нет -pthread): выполняем в текущем */
		bm_bench_body(&ctx);

	#else

		pthread_t th;
		if (pthread_create(&th, NULL, bench_thread_entry, &ctx) != 0) return -1;
		pthread_join(th, NULL);

	#endif

	*result = ctx.res;
	return 0;
}


/* ------------------------------------------------------------------ */
/* Вывод                                                              */
/* ------------------------------------------------------------------ */

#define BM_NS_TO_S(x) ((double)(x) / 1e9)

static inline void bm_bench_print(const bench_result *r) {
	printf("Wall time:        %llu ns  (%.6f s)\n",
	    (unsigned long long)r->wall_ns, BM_NS_TO_S(r->wall_ns));

	if (r->flags & BENCH_HAS_CPU) printf("CPU time:         %llu ns  (%.6f s)\n",
		    (unsigned long long)r->cpu_ns, BM_NS_TO_S(r->cpu_ns));
	else printf("CPU time:         n/a\n");

	if (r->flags & BENCH_HAS_SPLIT) {
		printf("  user:           %llu ns  (%.6f s)\n",
		    (unsigned long long)r->user_ns, BM_NS_TO_S(r->user_ns));
		printf("  system:         %llu ns  (%.6f s)\n",
		    (unsigned long long)r->sys_ns, BM_NS_TO_S(r->sys_ns));
	}
	if (r->flags & BENCH_HAS_CYCLES) printf("CPU cycles:       %llu  (%.3f M)\n",
		    (unsigned long long)r->cycles, (double)r->cycles / 1e6);

	if ((r->flags & BENCH_HAS_CPU) && r->wall_ns) printf("CPU utilization:  %.1f %%\n",
		    100.0 * (double)r->cpu_ns / (double)r->wall_ns);

	if (r->flags & BENCH_HAS_CTX) {
		printf("Ctx switches:     %llu voluntary, %llu involuntary\n",
		    (unsigned long long)r->vol_ctx,
		    (unsigned long long)r->invol_ctx);
	}
	if (r->flags & BENCH_HAS_FAULTS) {
		printf("Page faults:      %llu minor, %llu major\n",
		    (unsigned long long)r->minor_faults,
		    (unsigned long long)r->major_faults);
	}
	if (r->flags & BENCH_HAS_PEAK_MEM) printf("Peak memory:      %llu B  (%.2f MiB, whole process)\n",
		    (unsigned long long)r->peak_mem_bytes,
		    (double)r->peak_mem_bytes / (1024.0 * 1024.0));
	printf("Result:           %d\n", r->value);
}


/*
 * Запускает функцию в отдельном потоке и печатает результаты.
 *
 *     void *foo(void) { ...; return NULL; }
 *     bm_test(foo);
 */
static inline void bm_test(test_func func) {
	bench_result r;

	if (bm_bench_run(func, &r) != 0) {
		fprintf(stderr, "benchmark: failed to create thread\n");
		return;
	}

	bm_bench_print(&r);
}

#endif /* BENCHMARK_H */

/*
 * benchmark.h: замер времени и ресурсов потока
 *
 * ИСПОЛЬЗОВАНИЕ
 *
 *     #include "benchmark.h"
 *
 *     void *foo(void) { return NULL; }
 *
 *     int main(void) {
 *         bm_test(foo);
 *         return 0;
 *     }
 *
 * СБОРКА
 *
 *     Linux      gcc -O2 -pthread main.c -o main
 *     macOS      gcc -O2 -pthread main.c -o main
 *     MinGW      gcc -O2 main.c -o main.exe -lpsapi
 *     MSVC       cl /O2 main.c
 *     Emscripten emcc -O2 -pthread main.c -o main.js
 *                (без -pthread тест идёт в главном потоке)
 *
 * МЕТРИКИ ПО СИСТЕМАМ
 *
 *     Метрика              Linux  macOS  Windows  Emscripten
 *     Wall time              +      +       +         +
 *     CPU time               +      +       +         -
 *     User / System          +      +       +         -
 *     CPU cycles             -      -       +         -
 *     Context switches       +      -       -         -
 *     Page faults            +      -       -         -
 *     Peak memory (процесс)  +      +       +         +
 *
 * ПРИМЕЧАНИЯ
 *
 *     - Peak memory относится ко всему процессу, не к потоку.
 *     - Windows: _WIN32_WINNT >= 0x0600 для QueryThreadCycleTime.
 */