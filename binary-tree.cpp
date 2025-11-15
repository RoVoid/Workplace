#include <iostream>
#include <queue>

using std::cout;
using std::endl;
using std::nothrow;
using std::queue;

struct Node {
    int value;
    unsigned index;
    Node *left, *right;
};

class Tree {
    Node* anchor = nullptr;

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    public:
    ~Tree() { clear(anchor); }

    void push(const int& value) {
        Node* node = new (nothrow) Node{value, 0, nullptr, nullptr};
        if (!node) exit(2);

        if (!anchor) {
            anchor = node;
            anchor->index = 1;
            return;
        }

        Node* curr = anchor;
        while (1) {
            if (value <= curr->value) {
                if (!curr->left) {
                    node->index = curr->index * 2;
                    curr->left = node;
                    break;
                }
                curr = curr->left;
                continue;
            }
            if (!curr->right) {
                node->index = curr->index * 2 + 1;
                curr->right = node;
                break;
            }
            curr = curr->right;
        }
    }

    unsigned find(const int& value) const {
        if (anchor) {
            Node* curr = anchor;
            while (curr) {
                if (curr->value == value) return curr->index;
                curr = value < curr->value ? curr->left : curr->right;
            }
        }
        return 0;
    }

    void show() const {
        cout << "{ ";
        if (anchor) {
            queue<Node*> nodes;
            Node* curr;
            nodes.push(anchor);
            while (!nodes.empty()) {
                curr = nodes.front();
                nodes.pop();
                cout << curr->index << ':' << curr->value << ' ';
                if (curr->left) nodes.push(curr->left);
                if (curr->right) nodes.push(curr->right);
            }
        }
        cout << '}' << endl;
    }
};

int main() {
    Tree t;
    t.push(5);
    t.push(3);
    t.push(5);
    t.push(-1);
    t.push(40);
    t.push(35);
    t.push(-5);
    t.show();
    cout << t.find(-1) << endl;
    return 0;
}