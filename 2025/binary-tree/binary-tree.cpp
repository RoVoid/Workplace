#include "binary-tree.hpp"

#include <iostream>
#include <queue>

using std::cout;
using std::endl;
using std::nothrow;
using std::queue;

void Tree::clear(Node* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

void Tree::replaceInParent(Node* parent, Node* old, Node* _new) {
    if (!parent) anchor = _new;
    else if (parent->left == old) parent->left = _new;
    else parent->right = _new;

    if (_new) _new->parent = parent;
}

void Tree::childrenIndexing(Node* parent) {
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

Tree::~Tree() { clear(anchor); }

Node* Tree::root() const { return anchor; }

void Tree::push(const int& value) {
    Node* node = new (nothrow) Node{value, 0, nullptr, nullptr, nullptr};
    if (!node) exit(2);

    if (!anchor) {
        anchor = node;
        anchor->index = 1;
        return;
    }

    Node* curr = anchor;
    while (true) {
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

void Tree::remove(const int& value) {
    Node* node = anchor;
    Node* parent = nullptr;

    while (node && node->value != value) {
        parent = node;
        node = value < node->value ? node->left : node->right;
    }
    if (!node) return;

    if (!node->left && !node->right) {
        replaceInParent(parent, node, nullptr);
        delete node;
        return;
    }

    if (!node->left || !node->right) {
        Node* child = node->left ? node->left : node->right;
        child->index = node->index;
        replaceInParent(parent, node, child);
        childrenIndexing(child);
        delete node;
        return;
    }

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

unsigned Tree::find(const int& value) const {
    Node* curr = anchor;
    while (curr) {
        if (curr->value == value) return curr->index;
        curr = value < curr->value ? curr->left : curr->right;
    }
    return 0;
}

void Tree::show() const {
    cout << "{ ";
    if (anchor) {
        queue<Node*> nodes;
        nodes.push(anchor);

        while (!nodes.empty()) {
            Node* curr = nodes.front();
            nodes.pop();

            cout << curr->index << ":" << curr->value << ' ';

            if (curr->left) nodes.push(curr->left);
            if (curr->right) nodes.push(curr->right);
        }
    }
    cout << '}' << endl;
}
