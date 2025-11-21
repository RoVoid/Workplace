#include <iostream>
#include <new>

#define uint unsigned

using std::cout;
using std::endl;
using std::nothrow;

class BitSet {
    static constexpr uint SHIFT = 5;  // sizeof(unsigned) = 32 = 2^5
    static constexpr uint BITS = 1u << SHIFT;
    static constexpr uint MASK = BITS - 1;

    uint max, size, *a;

    void showElement(uint index) {
        if (index >= size) return;

        uint startBit = index << SHIFT;
        uint remaining = (max >= startBit) ? (max - startBit + 1) : 0;
        uint m = remaining < BITS ? remaining : BITS;

        // for (uint i = 0; i < m; ++i) cout << ((a[index] >> i) & 1 ? '1' : '0');
        for (uint i = 0; i < m; ++i)
            if ((a[index] >> i) & 1) cout << startBit + i << ' ';
    }

    public:
    BitSet(uint bits) : max(bits) {
        if (bits == 0) exit(2);

        size = (bits >> SHIFT) + 1;
        a = new (nothrow) uint[size];
        if (!a) exit(2);

        for (uint i = 0; i < size; ++i) a[i] = 0;
    }

    BitSet(const BitSet& other) : max(other.max), size(other.size) {
        a = new (nothrow) uint[size];
        if (!a) exit(2);
        for (uint i = 0; i < size; ++i) a[i] = other.a[i];
    }

    ~BitSet() { delete[] a; }

    void add(uint value) {
        if (value >= max) return;
        a[value >> SHIFT] |= 1u << (value & MASK);
    }

    void remove(uint value) {
        if (value >= max) return;
        a[value >> SHIFT] &= ~(1u << (value & MASK));
    }

    bool contain(uint value) const {
        if (value >= max) return false;
        return (a[value >> SHIFT] & (1u << (value & MASK))) != 0;
    }

    BitSet unions(const BitSet& other) const {
        BitSet r(max > other.max ? max : other.max);
        uint n = size < other.size ? size : other.size;
        for (uint i = 0; i < n; i++) r.a[i] = a[i] | other.a[i];
        return r;
    }

    BitSet intersections(const BitSet& other) const {
        BitSet r(max < other.max ? max : other.max);
        uint n = size < other.size ? size : other.size;
        for (uint i = 0; i < n; i++) r.a[i] = a[i] & other.a[i];
        return r;
    }

    void show() {
        cout << "[ ";
        for (uint i = 0; i < size; ++i) {
            showElement(i);
            // cout << ' ';
        }
        cout << "]\n";
    }
};

int main() {
    BitSet s1(36);
    cout << "Add: ";
    s1.add(1);
    s1.add(31);
    s1.add(7);
    s1.add(35);
    s1.show();
    cout << "Remove: ";
    s1.remove(7);
    s1.show();
    BitSet s2(22);
    s2.add(1);
    s2.add(5);
    s1.show();
    s2.show();
    cout << "Union: ";
    s1.unions(s2).show();  // Объединение
    cout << "Inter.: ";
    s1.intersections(s2).show();  // Пересечение
    return 0;
}
