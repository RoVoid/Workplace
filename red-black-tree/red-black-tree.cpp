#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

#define uint unsigned

using std::cout;
using std::endl;
using std::ifstream;
using std::nothrow;
using std::ofstream;
using std::ostringstream;
using std::queue;
using std::string;

struct Node {
    int value;
    bool isBlack;
    Node *left, *right, *parent;
};

struct IndexedNode {
    uint index;
    Node* node;
};

class RBTree {
    Node* root = nullptr;

    Node* grandparent(Node* n) const { return n && (n->parent) ? n->parent->parent : nullptr; }

    Node* uncle(Node* n) const {
        Node* g = grandparent(n);
        if (!g) return nullptr;
        return n->parent == g->left ? g->right : g->left;
    }

    Node* sibling(Node* n) const {
        if (!n || !n->parent) return nullptr;
        return n == n->parent->left ? n->parent->right : n->parent->left;
    }

    void rotate_left(Node* n) {
        Node* pivot = n->right;

        pivot->parent = n->parent;
        if (n->parent) {
            if (n->parent->left == n) n->parent->left = pivot;
            else n->parent->right = pivot;
        }

        n->right = pivot->left;
        if (pivot->left) pivot->left->parent = n;

        n->parent = pivot;
        pivot->left = n;

        if (root == n) root = pivot;
    }

    void rotate_right(Node* n) {
        Node* pivot = n->left;

        pivot->parent = n->parent;
        if (n->parent) {
            if (n->parent->left == n) n->parent->left = pivot;
            else n->parent->right = pivot;
        }

        n->left = pivot->right;
        if (pivot->right) pivot->right->parent = n;

        n->parent = pivot;
        pivot->right = n;

        if (root == n) root = pivot;
    }

    void replace_node(Node* n, Node* child) {
        if (!n->parent) { root = child; }
        else if (n == n->parent->left) { n->parent->left = child; }
        else { n->parent->right = child; }
        if (child) child->parent = n->parent;
    }

    // ------------ INSERT CASES ------------

    void insert_case1(Node* n) {
        if (n->parent) insert_case2(n);
        else n->isBlack = true;
    }

    void insert_case2(Node* n) {
        if (n->parent->isBlack) return;
        insert_case3(n);
    }

    void insert_case3(Node* n) {
        Node *u = uncle(n), *g;

        if (!u || u->isBlack) {
            insert_case4(n);
            return;
        }

        n->parent->isBlack = true;
        u->isBlack = true;
        g = grandparent(n);
        g->isBlack = false;
        insert_case1(g);
    }

    void insert_case4(Node* n) {
        Node* g = grandparent(n);

        if ((n == n->parent->right) && (n->parent == g->left)) {
            rotate_left(n->parent);
            n = n->left;
        }
        else if ((n == n->parent->left) && (n->parent == g->right)) {
            rotate_right(n->parent);
            n = n->right;
        }

        insert_case5(n);
    }

    void insert_case5(Node* n) {
        Node* g = grandparent(n);

        n->parent->isBlack = true;
        g->isBlack = false;
        if ((n == n->parent->left) && (n->parent == g->left)) rotate_right(g);
        else rotate_left(g);
    }

    // ------------ DELETE HELPERS ------------

    Node* minimum(Node* n) const {
        while (n->left) n = n->left;
        return n;
    }

    Node* find_node(int value) const {
        Node* cur = root;
        while (cur) {
            if (value < cur->value) cur = cur->left;
            else if (value > cur->value) cur = cur->right;
            else return cur;
        }
        return nullptr;
    }

    // ------------ DELETE CASES ------------

    void delete_case1(Node* n) {
        if (n->parent) delete_case2(n);
    }

    void delete_case2(Node* n) {
        Node* s = sibling(n);

        if (s && !s->isBlack) {
            n->parent->isBlack = false;
            s->isBlack = true;
            if (n == n->parent->left) rotate_left(n->parent);
            else rotate_right(n->parent);
        }
        delete_case3(n);
    }

    void delete_case3(Node* n) {
        Node* s = sibling(n);

        if (n->parent->isBlack && s && s->isBlack && (!s->left || s->left->isBlack) &&
            (!s->right || s->right->isBlack)) {
            s->isBlack = false;
            delete_case1(n->parent);
        }
        else { delete_case4(n); }
    }

    void delete_case4(Node* n) {
        Node* s = sibling(n);

        if (!n->parent->isBlack && s && s->isBlack && (!s->left || s->left->isBlack) &&
            (!s->right || s->right->isBlack)) {
            s->isBlack = false;
            n->parent->isBlack = true;
        }
        else { delete_case5(n); }
    }

    void delete_case5(Node* n) {
        Node* s = sibling(n);

        if (s && s->isBlack) {
            if (n == n->parent->left && (!s->right || s->right->isBlack) && (s->left && !s->left->isBlack)) {
                s->isBlack = false;
                s->left->isBlack = true;
                rotate_right(s);
            }
            else if (n == n->parent->right && (!s->left || s->left->isBlack) && (s->right && !s->right->isBlack)) {
                s->isBlack = false;
                s->right->isBlack = true;
                rotate_left(s);
            }
        }
        delete_case6(n);
    }

    void delete_case6(Node* n) {
        Node* s = sibling(n);

        if (!s) return;

        s->isBlack = n->parent->isBlack;
        n->parent->isBlack = true;

        if (n == n->parent->left) {
            if (s->right) s->right->isBlack = true;
            rotate_left(n->parent);
        }
        else {
            if (s->left) s->left->isBlack = true;
            rotate_right(n->parent);
        }
    }

    public:
    Node* head() const { return root; }

    void push(const int& value) {
        Node* node = new (nothrow) Node{value, false, nullptr, nullptr, nullptr};
        if (!node) return;
        if (!root) {
            root = node;
            insert_case1(node);
            return;
        }

        Node* curr = root;
        while (1) {
            if (curr->value > value) {
                if (!curr->left) {
                    node->parent = curr;
                    curr->left = node;
                    break;
                }
                curr = curr->left;
                continue;
            }
            if (!curr->right) {
                node->parent = curr;
                curr->right = node;
                break;
            }
            curr = curr->right;
        }
        insert_case1(node);
    }

    // ------------ REMOVE ------------

    void remove(int value) {
        Node* n = find_node(value);
        if (!n) return;

        Node* child;
        if (n->left && n->right) {
            Node* pred = minimum(n->right);
            n->value = pred->value;
            n = pred;
        }

        child = n->left ? n->left : n->right;
        bool n_black = n->isBlack;

        replace_node(n, child);

        if (child) child->isBlack = true;

        if (n_black) {
            if (child) delete_case1(child);
            else delete_case1(n);
        }

        delete n;
    }

    void show() {
        std::queue<IndexedNode> pairs;
        cout << "\\/\\/\\/\\/\\/" << endl;
        if (root) {
            IndexedNode pair;
            uint level = 0;
            pairs.push({0, root});
            while (!pairs.empty()) {
                pair = pairs.front();
                pairs.pop();
                if (level != pair.index) {
                    cout << endl;
                    ++level;
                }
                cout << pair.node->value << (pair.node->isBlack ? '+' : '-') << ' ';
                if (pair.node->left) pairs.push({pair.index + 1, pair.node->left});
                if (pair.node->right) pairs.push({pair.index + 1, pair.node->right});
            }
        }
        cout << endl << "/\\/\\/\\/\\/\\" << endl;
    }
};

class RBTreeHtml {
    string file, title;
    ostringstream out;

    void add_node(queue<IndexedNode>& nodes, IndexedNode& inode, bool is_left) {
        Node* child = is_left ? inode.node->left : inode.node->right;
        uint index = is_left ? inode.index * 2 : inode.index * 2 + 1;
        out << inode.index << "[\"" << inode.node->value;
        if (child) {
            nodes.push({index, child});
            out << "\"]---" << index << "[\"" << child->value << "\"]";
            if (!child->isBlack) out << ":::red";
            out << "\n";
        }
        else { out << "\"]~~~" << index << ":::hidden\n"; }
    }

    public:
    RBTreeHtml(string file = "", string title = "Красное-Чёрное Дерево")
        : file(file.empty() ? "rbtree.html" : file), title(title) {}

    void push(const RBTree& instance, const string& description = "") {
        out << "<div class=\"tree\">\n";
        if (!description.empty()) out << "<span>" << description << "</span>\n";
        if (!instance.head()) return;

        out << "<div class=\"mermaid\" align=\"center\">graph TD;\n";
        queue<IndexedNode> nodes;
        nodes.push({1, instance.head()});
        while (!nodes.empty()) {
            IndexedNode temp = nodes.front();
            nodes.pop();
            add_node(nodes, temp, true);
            add_node(nodes, temp, false);
        }
        out << "classDef hidden display: none; classDef red fill:red,color:#000;</div>\n</div>\n";
    }

    bool save() const {
        ofstream f(file);
        if (!f.is_open()) return false;

        f << "<!DOCTYPE html>\n"
        "<html lang=\"ru\">\n"
        "<head>\n"
        "<meta charset=\"UTF-8\">\n"
        "<title>" << title << "</title>\n"
        "<script type=\"module\">\n"
        "import mermaid from "
        "\"https://cdn.jsdelivr.net/npm/mermaid@10/dist/mermaid.esm.min.mjs\";\n"
        "mermaid.initialize({startOnLoad:true, flowchart:{useMaxWidth:false, curve:'linear', nodeSpacing:30, rankSpacing:30}});\n"
        "</script>\n"
        "<style>\n"
        "body { display: flex; flex-wrap: wrap; gap: 20px; background: #212121; color: white; }\n"
        ".tree { display: flex; flex-direction: column; background: #333; padding: 10px; border-radius: 10px; width: min-content; height: min-content; }\n"
        ".mermaid svg path {  stroke: white !important; }\n"
        ".mermaid .node rect { fill: #212121 !important; stroke: white !important; }\n"
        ".mermaid .nodeLabel { color: white !important; }\n"
        "span { text-align: center; margin-bottom: 5px; }\n"
        "</style>\n"
        "</head>\n<body>\n";
        f << out.str();
        f << "\n</body>\n</html>\n";

        return true;
    }
};

int main() {
    RBTree t;
    RBTreeHtml th;

    ifstream file("red-black-tree/input.txt");
    if (!file.is_open()) return 1;

    int value;
    while (file >> value) {
        if (value >= 0) {
            t.push(value);
            th.push(t, "Add: " + std::to_string(value));
        }
        else {
            t.remove(-value);
            th.push(t, "Remove: " + std::to_string(-value));
        }
    }

    if (t.head()) {
        t.show();
        th.save();
    }

    return 0;
}
