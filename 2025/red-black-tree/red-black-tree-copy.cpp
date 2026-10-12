#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

using namespace std;

struct Node {
    int value;
    bool black;
    Node *left, *right, *parent;
};

class RBTree {
    Node* root = nullptr;

    Node* grand(Node* n) { return n && n->parent ? n->parent->parent : nullptr; }
    Node* uncle(Node* n) {
        Node* g = grand(n);
        if (!g) return nullptr;
        return n->parent == g->left ? g->right : g->left;
    }
    Node* sibling(Node* n) {
        return n && n->parent ? (n == n->parent->left ? n->parent->right : n->parent->left) : nullptr;
    }

    void rotateLeft(Node* n) {
        Node* p = n->right;
        n->right = p->left;
        if (p->left) p->left->parent = n;
        p->parent = n->parent;
        if (!n->parent) root = p;
        else if (n == n->parent->left) n->parent->left = p;
        else n->parent->right = p;
        p->left = n;
        n->parent = p;
    }

    void rotateRight(Node* n) {
        Node* p = n->left;
        n->left = p->right;
        if (p->right) p->right->parent = n;
        p->parent = n->parent;
        if (!n->parent) root = p;
        else if (n == n->parent->left) n->parent->left = p;
        else n->parent->right = p;
        p->right = n;
        n->parent = p;
    }

    /* ---------- INSERT CASES ---------- */

    void insCase1(Node* n) {
        if (!n->parent) n->black = true;
        else insCase2(n);
    }

    void insCase2(Node* n) {
        if (n->parent->black) return;
        insCase3(n);
    }

    void insCase3(Node* n) {
        Node* u = uncle(n);
        Node* g;
        if (u && !u->black) {
            n->parent->black = u->black = true;
            g = grand(n);
            g->black = false;
            insCase1(g);
        }
        else insCase4(n);
    }

    void insCase4(Node* n) {
        Node* g = grand(n);
        if (n == n->parent->right && n->parent == g->left) {
            rotateLeft(n->parent);
            n = n->left;
        }
        else if (n == n->parent->left && n->parent == g->right) {
            rotateRight(n->parent);
            n = n->right;
        }
        insCase5(n);
    }

    void insCase5(Node* n) {
        Node* g = grand(n);
        n->parent->black = true;
        g->black = false;
        if (n == n->parent->left) rotateRight(g);
        else rotateLeft(g);
    }

    /* ---------- DELETE HELPERS ---------- */

    void replace(Node* a, Node* b) {
        if (!a->parent) root = b;
        else if (a == a->parent->left) a->parent->left = b;
        else a->parent->right = b;
        if (b) b->parent = a->parent;
    }

    Node* min(Node* n) {
        while (n->left) n = n->left;
        return n;
    }

    /* ---------- DELETE CASES ---------- */

    void delCase1(Node* n) {
        cout << "Delete 1\n";
        if (!n) return;
        if (n->parent) delCase2(n);
    }

    void delCase2(Node* n) {
        cout << "Delete 2\n";
        Node* s = sibling(n);
        if (s && !s->black) {
            n->parent->black = false;
            s->black = true;
            if (n == n->parent->left) rotateLeft(n->parent);
            else rotateRight(n->parent);
        }
        delCase3(n);
    }

    void delCase3(Node* n) {
        cout << "Delete 3\n";
        Node* s = sibling(n);
        if (n->parent->black && s && s->black && (!s->left || s->left->black) && (!s->right || s->right->black)) {
            s->black = false;
            delCase1(n->parent);
        }
        else delCase4(n);
    }

    void delCase4(Node* n) {
        cout << "Delete 4\n";
        Node* s = sibling(n);
        if (!n->parent->black && s && s->black && (!s->left || s->left->black) && (!s->right || s->right->black)) {
            s->black = false;
            n->parent->black = true;
        }
        else delCase5(n);
    }

    void delCase5(Node* n) {
        cout << "Delete 5\n";
        Node* s = sibling(n);
        if (s && s->black) {
            if (n == n->parent->left && (!s->right || s->right->black) && s->left && !s->left->black) {
                s->black = false;
                s->left->black = true;
                rotateRight(s);
            }
            else if (n == n->parent->right && (!s->left || s->left->black) && s->right && !s->right->black) {
                s->black = false;
                s->right->black = true;
                rotateLeft(s);
            }
        }
        delCase6(n);
    }

    void delCase6(Node* n) {
        cout << "Delete 6\n";
        try {
            Node* s = sibling(n);
            if (!s) return;
            s->black = n->parent->black;
            n->parent->black = true;
            if (n == n->parent->left) {
                if (s->right) s->right->black = true;
                rotateLeft(n->parent);
            }
            else {
                if (s->left) s->left->black = true;
                rotateRight(n->parent);
            }
        }
        catch (...) {
            cout << "error\n";
        }
        cout << "Delete 6 End\n";
    }

    public:
    void push(int v) {
        Node* n = new Node{v, false, nullptr, nullptr, nullptr};
        cout << "add " << v << endl;
        if (!root) {
            root = n;
            n->black = true;
            return;
        }
        Node* cur = root;
        Node* p = nullptr;
        while (cur) {
            p = cur;
            cur = v < p->value ? p->left : p->right;
        }
        n->parent = p;
        if (v < p->value) p->left = n;
        else p->right = n;
        insCase1(n);
    }

    void remove(int v) {
        Node* z = root;
        while (z && z->value != v) z = v < z->value ? z->left : z->right;
        if (!z) return;

        Node* y = z;
        Node* x;
        bool yBlack = y->black;

        if (!z->left) {
            x = z->right;
            replace(z, z->right);
        }
        else if (!z->right) {
            x = z->left;
            replace(z, z->left);
        }
        else {
            y = min(z->right);
            yBlack = y->black;
            x = y->right;
            if (y->parent != z) {
                replace(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            replace(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->black = z->black;
        }
        if (yBlack) delCase1(x ? x : y->parent);
        delete z;
    }

    Node* head() const { return root; }

    void show() {
        queue<Node*> q;
        if (!root) return;
        q.push(root);
        while (!q.empty()) {
            Node* n = q.front();
            q.pop();
            cout << n->value << (n->black ? '+' : '-') << ' ';
            if (n->left) q.push(n->left);
            if (n->right) q.push(n->right);
        }
        cout << endl;
    }
};

struct IndexedNode {
    unsigned index;
    Node* node;
};

class RBTreeHtml {
    string file, title;
    ostringstream out;

    void add_node(queue<IndexedNode>& nodes, IndexedNode& inode, bool is_left) {
        Node* child = is_left ? inode.node->left : inode.node->right;
        unsigned index = is_left ? inode.index * 2 : inode.index * 2 + 1;
        out << inode.index << "[\"" << inode.node->value;
        if (child) {
            nodes.push({index, child});
            out << "\"]---" << index << "[\"" << child->value << "\"]";
            if (!child->black) out << ":::red";
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
    RBTreeHtml th("red-black-tree/rbtree.html");

    ifstream file("red-black-tree/input.txt");
    if (!file.is_open()) return 1;

    int value;
    try {
        while (file >> value) {
            if (value >= 0) {
                t.push(value);
                th.push(t, "Add: " + std::to_string(value));
            }
            else {
                cout << value << " try remove\n";

                t.remove(-value);
                cout << "remove\n";
                th.push(t, "Remove: " + std::to_string(-value));
            }
        }
    }
    catch (...) {
        cout << "er";
    }

    // if (t.head()) {
    cout << "1\n";
    t.show();
    cout << "2\n";
    th.save();
    cout << "3\n";
    // }

    return 0;
}