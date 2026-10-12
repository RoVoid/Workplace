#include <iostream>
#include <new>

#define uint unsigned

using std::cout;
using std::endl;
using std::nothrow;

class bit {
    uint Nmax, size, *a;

    public:
    bit(unsigned int n) {
        Nmax = n;
        size = (Nmax >> 5) + 1;
        a = new (nothrow) unsigned int[size];

        if (!a) { exit(1); }

        for (unsigned int i = 0; i < size; i++) a[i] = 0;
    }

    ~bit() { delete[] a; }

    bool ok(unsigned int x) { return x <= Nmax; }

    bool add(unsigned int x) {
        if (!ok(x)) return false;

        a[x >> 5] |= (1u << (x & 31));
        return true;
    }

    bool contain(unsigned int x) {
        if (!ok(x)) return false;
        return ((a[x >> 5]) & (1u << (x & 31)));
    }

    void show() {
        cout << "{ ";
        for (unsigned int i = 0; i <= Nmax; i++) {
            if (contain(i)) cout << i << " ";
        }
        cout << "}" << endl;
    }

    bit unions(const bit& other) const {
        if (size < other.size) return other.unions(*this);  // = зациклится
        bit res(Nmax);
        for (unsigned i = 0; i < other.size; i++) res.a[i] = a[i] | other.a[i];  // unsigned int ~ unsigned
        for (unsigned i = other.size; i < size; i++) res.a[i] = a[i];
        res.show();
        return res;
    }
};

int main() {
    bit ob1(3);
    ob1.show();
    ob1.add(52);
    ob1.add(7);
    ob1.add(3);
    ob1.add(10);
    ob1.add(15);
    ob1.show();
    ob1.contain(7);
    ob1.contain(0);
    // ob1.remove(10);
    ob1.show();

    bit ob2(10);
    ob2.add(10);
    ob2.add(5);
    ob2.add(0);
    ob2.add(150);
    ob2.show();

    ob2.unions(ob1).show();

    // ob1.clear();
    ob1.show();
    return 0;
}
