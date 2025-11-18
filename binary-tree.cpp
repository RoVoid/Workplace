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

    // Учитываю случай с корнем
    void replaceInParent(Node* parent, Node* old, Node* _new) {
        if (!parent) anchor = _new;
        else if (parent->left == old) parent->left = _new;
        else parent->right = _new;
        if (_new) _new->parent = parent;
    }

    void childrenIndexing(Node* parent) {
        std::queue<Node*> q;
        q.push(parent);
        while (!q.empty()) {
            Node* c = q.front();
            q.pop();
            if (c->left) {
                c->left->index = c->index * 2;
                q.push(c->left);
            }
            if (c->right) {
                c->right->index = c->index * 2 + 1;
                q.push(c->right);
            }
        }
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
        Node* node = anchor;
        Node* parent = nullptr;

        // Поиск элемента
        while (node && node->value != value) {
            parent = node;
            node = value < node->value ? node->left : node->right;
        }
        if (!node) return;

        // Удаление если не родитель
        if (!node->left && !node->right) {
            replaceInParent(parent, node, nullptr);
            delete node;
            return;
        }

        // Удаление при одном потомке
        if (!node->left || !node->right) {
            Node* child = node->left ? node->left : node->right;
            child->index = node->index;
            replaceInParent(parent, node, child);
            childrenIndexing(child);
            delete node;
            return;
        }

        // Удаление при обоих
        // Вместо поиска максимума беру минимум правого потомка (направо и до конца влево)
        // Можно конечно начать слева потом до конца направо
        Node* del = node->right;
        while (del->left) del = del->left;
        node->value = del->value;

        replaceInParent(del->parent, del, del->right);
        if (del->right) {
            del->right->index = del->index;
            childrenIndexing(del->right);
        }
        delete del;
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

    Tree t1;
    t1.push(5);
    t1.show();
    t1.remove(5);
    t1.show();
    t1.push(3);
    t1.push(40);
    t1.push(-1);
    t1.push(4);
    t1.show();
    t1.remove(5);
    t1.remove(3);
    t1.show();
    return 0;
}