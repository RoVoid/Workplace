#include <iostream>
#include <new>

using std::cout;
using std::endl;

class array {
    int  si;
    int* mas;

    public:
    array(int size) : si(0) {
        if (size <= 0) {
            cout << "Invalid size" << endl;
            exit(1);
        }
        mas = new (std::nothrow) int[size]{0};
        if (!mas) {
            cout << "Memory not allocated";
            exit(1);
        }
        si = size;
        for (int i = 0; i < si; ++i) mas[i] = 0;
    }
    array(const array& other) {  // Ссылка нужна чтобы не создавать рекурсию конструктора копирования
        mas = new (std::nothrow) int[other.si];
        if (!mas) {
            cout << "Memory not allocated";
            exit(1);
        }
        si = other.si;
        for (int i = 0; i < si; ++i) mas[i] = other.mas[i];
    }
    ~array() { delete[] mas; }
    void put(int i, int x) {
        if (i < 0 || i >= si) {
            cout << "Out of range" << endl;
            exit(1);
        }
        mas[i] = x;
    }
    int get(int i) const {
        if (i < 0 || i >= si) {
            cout << "Out of range" << endl;
            exit(1);
        }
        return mas[i];
    }

    int& ist(int i) {  // Даёт доступ к ячейке массива
        if (i < 0 || i >= si) {
            cout << "Out of range" << endl;
            exit(1);
        }
        return mas[i];
    }
    int size() { return si; }
};

int main() {
    array a(5);

    a.ist(1) = 4;
    cout << a.get(1) << endl;

    a.ist(2) += 2;

    cout << a.get(2) << endl;

    a.put(0, 1);  // ok
    // a.put(5, 0);  // error

    for (int i = 0; i < a.size(); ++i) cout << a.get(i) << ' ';
    cout << endl;

    array b=a;

    b.put(4, 5);
    for (int i = 0; i < b.size(); ++i) cout << b.get(i) << ' ';
    cout << endl;

    return 0;
}
