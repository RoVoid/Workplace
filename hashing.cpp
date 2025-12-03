#include <iostream>
#include <list>
#include <string>
#include <vector>


class HashSet {
    private:
    int p;                                      // число классов
    std::vector<std::list<std::string>> table;  // хеш-таблица на списках

    int hash(const std::string& s) const {
        int sum = 0;
        for (unsigned char c : s) sum += c;
        return sum % p;
    }

    public:
    HashSet(int p) : p(p), table(p) {}

    // Добавление строки
    void insert(const std::string& s) {
        int h = hash(s);
        for (const auto& x : table[h])
            if (x == s) return;  // множество: дубликаты не храним
        table[h].push_back(s);
    }

    // Удаление строки
    void erase(const std::string& s) {
        int h = hash(s);
        table[h].remove(s);
    }

    // Поиск строки
    bool find(const std::string& s) const {
        int h = hash(s);
        for (const auto& x : table[h])
            if (x == s) return true;
        return false;
    }

    // Вывод таблицы
    void print() const {
        for (int i = 0; i < p; ++i) {
            std::cout << i << ": ";
            bool first = true;
            for (const auto& x : table[i]) {
                if (!first) std::cout << ", ";
                std::cout << x;
                first = false;
            }
            std::cout << "\n";
        }
    }
};

int main() {
    int p;
    std::cout << "Введите p (кол-во классов): ";
    std::cin >> p;

    HashSet hs(p);

    while (true) {
        std::cout << "\n1 - добавить\n" "2 - удалить\n" "3 - найти\n" "4 - вывести\n" "0 - выход\n" "Ваш выбор: ";

        int cmd;
        std::cin >> cmd;

        if (cmd == 0) break;

        std::string s;
        switch (cmd) {
        case 1:
            std::cout << "Строка: ";
            std::cin >> s;
            hs.insert(s);
            break;

        case 2:
            std::cout << "Строка: ";
            std::cin >> s;
            hs.erase(s);
            break;

        case 3:
            std::cout << "Строка: ";
            std::cin >> s;
            std::cout << (hs.find(s) ? "Найдена" : "Не найдена") << "\n";
            break;

        case 4: hs.print(); break;

        default: std::cout << "Неизвестная команда\n";
        }
    }

    return 0;
}
