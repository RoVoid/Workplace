#include <iostream>
#include <stdexcept>
using namespace std;

#define uint unsigned

struct Node {
    int value;
    Node *previous, *next;
};

class List {
    Node *start = nullptr, *last = nullptr;
    int _size = 0;

    public:
    ~List() {
        Node* e = start;
        while (e) {
            Node* next = e->next;
            delete e;
            e = next;
        }
    }

    void push(const int& value) {
        Node* e = new Node{value, last, nullptr};
        if (!start) start = e;
        if (last) last->next = e;
        last = e;
        ++_size;
    }

    void pop() {
        if (!last) return;
        Node* e = last;
        last = last->previous;
        if (last) last->next = nullptr;
        else start = nullptr;
        delete e;
        --_size;
    }

    void insert(const int& value, uint index) {
        if (index >= size()) {
            push(value);
            return;
        }
        Node* e = start;
        for (; index > 0; --index) e = e->next;
        Node* n = new Node{value, e->previous, e};
        if (e->previous) e->previous->next = n;
        else start = n;
        e->previous = n;
        ++_size;
    }

    void remove(uint index) {
        if (index >= size()) {
            pop();
            return;
        }
        Node* e = start;
        for (; index > 0; --index) e = e->next;
        e->previous->next = e->next;
        e->next->previous = e->previous;
        delete e;
        --_size;
    }

    int& operator[](uint index) {
        if (index >= size()) throw out_of_range("index");
        Node* e = start;
        for (; index > 1; --index) e = e->next;
        return e->value;
    }

    const int& operator[](uint index) const {
        if (index >= size()) throw out_of_range("index");
        Node* e = start;
        for (; index > 1; --index) e = e->next;
        return e->value;
    }

    int size() const { return _size; }
    bool empty() const { return _size == 0; }

    void show() const {
        Node* e = start;
        cout << '(' << size() << "){ ";
        while (e) {
            cout << e->value << ' ';
            e = e->next;
        }
        cout << '}' << endl;
    }
};

int main() {
    List l1;
    l1.show();
    for (uint i = 0; i < 10; ++i) l1.push(i);
    l1.show();
    cout << l1[4] << endl;
    l1.pop();
    l1.show();
    l1[0] = 10;
    l1.show();
    l1.insert(-1, 5);
    l1.show();
    l1.remove(7);
    l1.show();
    return 0;
}