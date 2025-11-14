#include <iostream>
#include <queue>

using namespace std;

struct Node {
    int value;
    Node *left, *right;
};

struct NodePair {
    unsigned index;
    Node* node;
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
        Node* node = new (nothrow) Node{value, nullptr, nullptr};
        if (!node) exit(2);

        if (!anchor) {
            anchor = node;
            return;
        }

        Node* curr = anchor;
        while (1) {
            if (value <= curr->value) {
                if (!curr->left) {
                    curr->left = node;
                    break;
                }
                curr = curr->left;
                continue;
            }
            if (!curr->right) {
                curr->right = node;
                break;
            }
            curr = curr->right;
        }
    }
    void show() const {
        cout << "{ ";
        if (anchor) {
            queue<NodePair> nodes;
            NodePair curr;
            nodes.push({1, anchor});
            while (!nodes.empty()) {
                curr = nodes.front();
                nodes.pop();
                cout << curr.index << ':' << curr.node->value << ' ';
                if (curr.node->left) nodes.push({curr.index * 2, curr.node->left});
                if (curr.node->right) nodes.push({curr.index * 2 + 1, curr.node->right});
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
    return 0;
}