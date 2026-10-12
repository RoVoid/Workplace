#include <iostream>

#define uint unsigned

template <typename A = int>
class Stack {
    A* a = nullptr;
    uint size, storage;

    public:
    Stack(const uint& n) : storage(n), size(0) {
        try {
            a = new A[n];
        }
        catch (...) {
            std::cout << "Ошибка выделения памяти!" << std::endl;
            return;
        }
    }
    Stack(const Stack& other) : storage(other.storage), size(other.size) {
        try {
            a = new A[size];
        }
        catch (...) {
            std::cout << "Ошибка выделения памяти!" << std::endl;
            return;
        }
        for (uint i = 0; i < size; ++i) a[i] = other.a[i];
    }
    ~Stack() { delete[] a; }

    bool push(const A& value) {
        if (full()) return false;
        a[size++] = value;
        return true;
    }

    bool pop() {
        if (empty()) return false;
        --size;
        return true;
    }

    A back() {
        if (empty()) {
            std::cout << "Стек пустой!" << std::endl;
            return NULL;
        }
        return a[size - 1];
    }

    int length() { return size; }

    bool empty() { return size == 0; }

    bool full() { return size == storage; }

    void show() {
        for (uint i = 0; i < size; ++i) { std::cout << a[i] << ' '; }
        std::cout << std::endl;
    }

    Stack operator+(const Stack& other) {
        Stack result(storage + other.storage);
        for (uint i = 0; i < size; ++i) result.a[i] = a[i];
        for (uint i = 0; i < other.size; ++i) result.a[i + size] = other.a[i];
        result.size = size + other.size;
        return result;
    }

    Stack& operator=(const Stack& other) {
        if (this == &other) {
            std::cout << "Передан сам объект!" << std::endl;
            return *this;
        }
        delete[] a;
        size = other.size;
        storage = other.storage;
        try {
            a = new A[size];
        }
        catch (...) {
            a = nullptr;
            std::cout << "Ошибка выделения памяти!" << std::endl;
            return *this;
        }
        for (uint i = 0; i < size; ++i) a[i] = other.a[i];
        return *this;
    }

    operator bool() { return a && !empty(); }
};

int main() {
    uint n;
    std::cin >> n;

    Stack s1(n);

    for (int i = 0; s1.push(i); ++i);

    if (!s1.push(999)) std::cout << "Стек полон!\n";

    std::cout << "Вершина s1: " << s1.back() << std::endl;

    std::cout << (s1.full() ? "s1 полон\n" : "s1 не полон\n");
    std::cout << (s1.empty() ? "s1 пуст\n" : "s1 не пуст\n");

    Stack<int> s2 = s1;

    s2.pop();
    s2.pop();

    std::cout << "s1: ";
    s1.show();
    std::cout << "s2: ";
    s2.show();

    std::cout << "s1 + s2: ";
    (s1 + s2).show();

    Stack s3(5);
    s3 = s1;
    std::cout << "s3: ";
    s3.show();
    s3 = s3;  // самоприсваивание

    if (s3) std::cout << "s4 не пуст и корректен\n";
    else std::cout << "s4 пуст или повреждён\n";

    Stack s4(3);
    std::cout << (s4.pop() ? "Убран элемент из s4\n" : "s4 пуст\n");
    s4.back();

    return 0;
}
