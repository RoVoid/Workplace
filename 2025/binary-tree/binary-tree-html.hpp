#ifndef TREE_HTML_HPP
#define TREE_HTML_HPP

#include <queue>
#include <sstream>
#include <string>

#include "binary-tree.hpp"

class TreeHtml {
    std::string file, title;
    std::ostringstream out;

    void add_node(std::queue<Node*>& nodes, Node*& node, Node* child, bool is_left);

    public:
    TreeHtml(std::string file = "", std::string title = "Бинарное Дерево");

    // Добавление дерева к HTML с опциональным описанием
    void push(const Tree& instance, const std::string& description = "");

    // Сохранение HTML в файл
    bool save() const;

    // Открыть HTML в браузере (Windows)
    bool open() const;
};

#endif