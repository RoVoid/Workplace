#include <iostream>

#include "binary-tree-html.hpp"

using std::cout;
using std::endl;

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

    TreeHtml th;
    th.push(t, "Первое дерево");
    th.push(t1, "Второе дерево");

    t.push(1);
    th.push(t, "Добавил 1 в первое дерево");

    Tree t2;
    th.push(t2, "Пустое дерево");
    t2.push(0);
    th.push(t2, "Дерево с одним элементов");

    th.save();
    // th.open(); // У меня по умолчанию VS Code
    return 0;
}

// crun .\binary-tree\ -n binary-tree -c
// g++ .\binary-tree\* -o binary-tree && .\binary-tree