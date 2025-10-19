#include <iostream>

#define uint unsigned

template <typename A = int>
class Stack {
    A* a;
    uint size, storage;

    public:
    Stack(const uint& n) : storage(n), size(0) {
        try {
            a = new A[n];
        }
        catch (...) {
            std::cout << "Ошибка выделения памяти!";
            return;
        }
    }
    Stack(const Stack& other) : storage(other.storage), size(other.size) {
        try {
            a = new A[size];
        }
        catch (...) {
            std::cout << "Ошибка выделения памяти!";
            return;
        }
        for (uint i = 0; i < size; ++i) a[i] = other.a[i];
    }
    ~Stack() { delete[] a; }

    bool push(const A& value) {
        if (storage <= size) return false;
        a[size++] = value;
        return true;
    }

    bool pop() {
        if (size == 0) return false;
        --size;
        return true;
    }

    A back() { return size == 0 ? NULL : a[size - 1]; }

    int length() { return size; }

    void show() {
        for (uint i = 0; i < size; ++i) { std::cout << a[i] << ' '; }
        std::cout << std::endl;
    }

    Stack operator+(const Stack& other) {
        Stack result(storage + other.storage);  // не понятно
        for (uint i = 0; i < size; ++i) result.a[i] = a[i];
        for (uint i = 0; i < other.size; ++i) result.a[i + size] = other.a[i];
        result.size = size + other.size;
        return result;
    }
};

int main() {
    uint n;
    std::cin >> n;
    Stack s1(n), s2(n + 1);

    for (int i = 0; s1.push(i); ++i);
    s2.push(n);
    s2.push(n + 1);

    s1.show();
    s2.show();
    (s1 + s2).show();

    return 0;
}