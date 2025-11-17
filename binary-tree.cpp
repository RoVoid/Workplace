#include <iostream>
#include <queue>

using std::cout;
using std::endl;
using std::nothrow;
using std::queue;

struct Node {
    int value;
    unsigned index;
    Node *left, *right, *parent;
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
        Node* node = new (nothrow) Node{value, 0, nullptr, nullptr, nullptr};
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
                    node->parent = curr;
                    curr->left = node;
                    break;
                }
                curr = curr->left;
                continue;
            }
            if (!curr->right) {
                node->index = curr->index * 2 + 1;
                node->parent = curr;
                curr->right = node;
                break;
            }
            curr = curr->right;
        }
    }

    void remove(const int& value) {
        // Поиск удаляемого
        Node* node = anchor;
        while (node) {
            if (node->value == value) break;
            node = value < node->value ? node->left : node->right;
        }
        if (!node || node->value != value) return;

        // Поиск максимального
        Node *max = node, *curr;
        queue<Node*> nodes;
        nodes.push(max);
        while (!nodes.empty()) {
            curr = nodes.front();
            nodes.pop();
            if (!(curr->left && curr->right) && max->value <= curr->value) { max = curr; }
            if (curr->left) nodes.push(curr->left);
            if (curr->right) nodes.push(curr->right);
        }
        cout << "Node: " << node->index << ' ' << node->value << endl;
        node->value = max->value;

        cout << "Max: " << max->index << ' ' << max->value << endl;

        // Удаление потомка
        Node* del = nullptr;
        if (max->left) del = max->left;
        else if (max->right) del = max->right;
        else {
            (max->parent->left == max ? max->parent->left : max->parent->right) = nullptr;
            delete max;
            return;
        }

        cout << "Del: " << del->index << ' ' << del->value << endl;

        max->value = del->value;
        if (del->left) {
            max->left = del->left;
            del->left->parent = max;
        }
        if (del->right) {
            max->right = del->right;
            del->right->parent = max;
        }
        (del->parent->left == del ? del->parent->left : del->parent->right) = nullptr;
        delete del;

        // Изменяем индексы потомкам
        nodes.push(max);
        while (!nodes.empty()) {
            curr = nodes.front();
            nodes.pop();
            if (curr->left) {
                curr->left->index = curr->index * 2;
                nodes.push(curr->left);
            }
            if (curr->right) {
                curr->right->index = curr->index * 2 + 1;
                nodes.push(curr->right);
            }
        }
    }

    unsigned find(const int& value) const {
        Node* curr = anchor;
        while (curr) {
            if (curr->value == value) return curr->index;
            curr = value < curr->value ? curr->left : curr->right;
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
    t.push(4);
    t.push(-1);
    t.push(40);
    t.push(35);
    t.push(-5);
    t.show();
    cout << t.find(-1) << endl;
    cout << "--------------\n";
    t.remove(3);
    t.show();
    t.remove(35);
    t.show();
    t.remove(35);
    t.show();
    return 0;
}