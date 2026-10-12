#ifndef TREE_HPP
#define TREE_HPP

struct Node {
    int value;
    unsigned index;
    Node *left, *right, *parent;
};

class Tree {
    Node* anchor = nullptr;

    void clear(Node* node);
    void replaceInParent(Node* parent, Node* old, Node* _new);
    void childrenIndexing(Node* parent);

    public:
    ~Tree();

    Node* root() const;
    void push(const int& value);
    void remove(const int& value);
    unsigned find(const int& value) const;
    void show() const;
};

#endif
