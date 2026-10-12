#include "binary-tree-html.hpp"

#include <cstdlib>
#include <fstream>

using std::ostringstream;
using std::queue;
using std::string;
using std::to_string;

void TreeHtml::add_node(queue<Node*>& nodes, Node*& node, Node* child, bool is_left) {
    if (child) {
        nodes.push(child);
        out << node->index << "[\"" << node->value << "\"]---" << child->index << "[\"" << child->value << "\"]\n";
    }
    else {
        unsigned idx = is_left ? node->index * 2 : node->index * 2 + 1;
        out << node->index << "[\"" << node->value << "\"]~~~" << idx << ":::hidden\n";
    }
}

TreeHtml::TreeHtml(string file, string title) : file(file.empty() ? "tree.html" : file), title(title) {}

void TreeHtml::push(const Tree& instance, const string& description) {
    out << "<div class=\"tree\">\n";
    if (!description.empty()) out << "<span>" << description << "</span>\n";

    out << "<div class=\"mermaid\" align=\"center\">graph TD;\n";

    if (instance.root()) {
        queue<Node*> nodes;
        nodes.push(instance.root());
        while (!nodes.empty()) {
            Node* node = nodes.front();
            nodes.pop();

            add_node(nodes, node, node->left, true);
            add_node(nodes, node, node->right, false);
        }
    }

    out << "classDef hidden display: none;</div>\n</div>\n";
}

bool TreeHtml::save() const {
    std::ofstream f(file);
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

bool TreeHtml::open() const { return system(("start \"\" \"" + file + "\"").c_str()) == 0; }
