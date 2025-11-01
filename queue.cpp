#include <iostream>

#define uint unsigned

class Queue {
    int* a = nullptr;
    uint start, end, storage;

    public:
    Queue(uint n) : storage(n), start(0), end(0) {
        try {
            a = new int[n];
        }
        catch (...) {
            std::cout << "Ошибка выделения памяти!" << std::endl;
            a = nullptr;
        }
    }

    Queue(const Queue& other) : storage(other.storage), start(other.start), end(other.end) {
        try {
            a = new int[storage];
        }
        catch (...) {
            std::cout << "Ошибка выделения памяти!" << std::endl;
            a = nullptr;
            return;
        }
        for (uint i = 0; i < storage; ++i) a[i] = other.a[i];
    }

    ~Queue() { delete[] a; }

    bool push(int value) {
        if (full()) return false;
        a[end] = value;
        end = (end + 1) % storage;
        return true;
    }

    bool pop() {
        if (empty()) return false;
        start = (start + 1) % storage;
        return true;
    }

    int front() const {
        if (empty()) {
            std::cout << "Очередь пустая!" << std::endl;
            return 0;
        }
        return a[start];
    }

    int back() const {
        if (empty()) {
            std::cout << "Очередь пустая!" << std::endl;
            return 0;
        }
        return a[(end + storage - 1) % storage];
    }

    uint length() const { return (end + storage - start) % storage; }

    bool empty() const { return start == end; }

    bool full() const { return (end + 1) % storage == start; }

    void show() const {
        uint len = length();
        for (uint i = 0; i < len; ++i) std::cout << a[(start + i) % storage] << ' ';
        std::cout << std::endl;
    }

    Queue operator+(const Queue& other) const {
        Queue result(storage + other.storage);
        uint len1 = length();
        uint len2 = other.length();
        for (uint i = 0; i < len1; ++i) result.push(a[(start + i) % storage]);
        for (uint i = 0; i < len2; ++i) result.push(other.a[(other.start + i) % other.storage]);
        return result;
    }

    Queue& operator=(const Queue& other) {
        if (this == &other) return *this;
        delete[] a;
        storage = other.storage;
        start = other.start;
        end = other.end;
        try {
            a = new int[storage];
        }
        catch (...) {
            std::cout << "Ошибка выделения памяти!" << std::endl;
            a = nullptr;
            return *this;
        }
        for (uint i = 0; i < storage; ++i) a[i] = other.a[i];
        return *this;
    }

    operator bool() const { return a && !empty(); }
};

int main() {
    uint n;
    std::cout << "Введите размер очереди: ";
    std::cin >> n;

    Queue q1(n);
    std::cout << "Добавляем элементы: ";
    for (int i = 1; q1.push(i * 10); ++i);
    q1.show();

    std::cout << "Попытка добавить 999: ";
    if (!q1.push(999)) std::cout << "Очередь полна\n";

    std::cout << "Передний элемент: " << q1.front() << "\n";
    std::cout << "Задний элемент: " << q1.back() << "\n";

    std::cout << "Размер: " << q1.length() << "\n";
    std::cout << (q1.full() ? "Очередь полна\n" : "Очередь не полна\n");
    std::cout << (q1.empty() ? "Очередь пуста\n" : "Очередь не пуста\n");

    std::cout << "Удаляем два элемента\n";
    q1.pop();
    q1.pop();
    q1.show();

    std::cout << "Копирование очереди q1 -> q2\n";
    Queue q2 = q1;
    q2.show();

    std::cout << "Оператор + (объединение q1 и q2)\n";
    Queue q3 = q1 + q2;
    q3.show();

    std::cout << "Оператор присваивания q4 = q1\n";
    Queue q4(5);
    q4 = q1;
    q4.show();

    std::cout << "Самоприсваивание q4 = q4\n";
    q4 = q4;
    q4.show();

    std::cout << "Проверка оператора bool\n";
    if (q4) std::cout << "q4 корректен и не пуст\n";
    else std::cout << "q4 пуст или повреждён\n";

    std::cout << "Проверка пустой очереди q5\n";
    Queue q5(3);
    std::cout << (q5.pop() ? "Удалён элемент\n" : "Очередь пуста\n");
    q5.back();

    return 0;
}
