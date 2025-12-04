#include <cstdlib>
#include <fstream>
#include <iostream>
#include <new>
#include <ostream>
#include <string>
#include <vector>

using std::ifstream;
using std::nothrow;
using std::string;
using std::vector;

using std::cout;
using std::endl;

struct Word {
    string value;
    Word* next;
};

struct Sentence {
    Word *head, *tail;
    Sentence* next;
    unsigned count;
};

class GetWord {
    Sentence* start = nullptr;
    unsigned sCount = 0;

    static void push_word_to_sentence(const string& value, Sentence*& sentence) {
        if (value.empty()) return;
        Word* word = new (nothrow) Word{value, nullptr};
        if (!word) exit(2);

        ++sentence->count;
        if (!sentence->tail) {
            sentence->head = sentence->tail = word;
            return;
        }

        sentence->tail->next = word;
        sentence->tail = word;
    }

    inline static bool is_delim(char c) {
        return c == ' ' || c == '\n' || c == '\t' || c == ',' || c == '"' || c == '\'';
    }

    public:
    GetWord(const string& file) {
        ifstream f(file);
        if (!f.is_open()) exit(1);

        start = new (nothrow) Sentence{nullptr, nullptr, nullptr, 0};
        if (!start) exit(2);

        Sentence* s = start;
        string str;
        char c;
        bool checkEnd = false;
        while (f.get(c)) {
            if (is_delim(c)) {
                push_word_to_sentence(str, s);
                str.clear();
                if (checkEnd && s->count) {
                    ++sCount;
                    Sentence* s1 = new (nothrow) Sentence{nullptr, nullptr, 0};
                    if (!s1) exit(4);
                    s->next = s1;
                    s = s1;
                    checkEnd = false;
                }
            }
            else if (c == '.') { checkEnd = true; }
            else {
                if (checkEnd) {
                    str.push_back('.');
                    checkEnd = false;
                }
                str.push_back(c);
            }
        }
        push_word_to_sentence(str, s);
        if (s->count) ++sCount;
    }

    ~GetWord() {
        Sentence* s = start;
        while (s) {
            Word* w = s->head;
            while (w) {
                Word* nxt = w->next;
                delete w;
                w = nxt;
            }

            Sentence* nextS = s->next;
            delete s;
            s = nextS;
        }
    }

    class iterator {
        Sentence* s;
        Word* w;

        public:
        iterator(Sentence* s_ = nullptr, Word* w_ = nullptr) : s(s_), w(w_) {}
        iterator& next() {
            if (!w) return *this;
            if (s->tail == w) {
                s = s->next;
                w = s ? s->head : nullptr;
            }
            else w = w->next;
            return *this;
        }
        iterator& skip() {
            if (!s) return *this;
            s = s->next;
            return *this;
        }
        Sentence sentence() const { return s ? *s : Sentence{}; }
        Word word() const { return w ? *w : Word{}; }
        string get() const { return w ? w->value : ""; }
        operator bool() const { return s && w; }
    };

    iterator begin() const { return iterator(start, start ? start->head : nullptr); }

    void print() const {
        cout << '{';
        if (!start) {
            cout << " }" << endl;
            return;
        }

        Sentence* s = start;
        while (s && s->count) {
            cout << "\n [ ";
            Word* w = s->head;
            while (w) {
                cout << w->value.size() << ':' << w->value << ' ';
                w = w->next;
            }
            cout << ']';
            s = s->next;
        }

        cout << "\n}" << endl;
    }

    inline unsigned sentenceCount() const { return sCount; }
    unsigned wordCount() const {
        unsigned count = 0;
        Sentence* s = start;
        while (s) {
            count += s->count;
            s = s->next;
        }
        return count;
    }
};

int main() {
    GetWord gw1("get-word/input1.txt");
    GetWord gw2("get-word/input2.txt");

    // 1) Количество слов в двух файлах и сравнение
    unsigned wc1 = gw1.wordCount();
    unsigned wc2 = gw2.wordCount();
    cout << "1) Words in input1: " << wc1 << endl;
    cout << "   Words in input2: " << wc2 << endl;
    cout << "   input1 " << (wc1 == wc2 ? '=' : wc1 > wc2 ? '>' : '<') << " input2" << endl;

    // 2) Количество предложений в файле input1
    unsigned sc1 = gw1.sentenceCount();
    cout << "2) Sentences in input1: " << sc1 << endl;

    // 3) Среднее количество слов в предложении
    cout << "3) Average words per sentence in input1: " << (sc1 ? wc1 / (float)sc1 : 0) << endl;

    // 4) Для каждого слова вывести длину
    cout << "4) Word lengths (input1):" << endl;
    for (auto it = gw1.begin(); it; it.next()) {
        string w = it.get();
        cout << w.size() << ": " << w << endl;
    }

    // 5) Слова длиной > N
    cout << "5) Enter N: ";
    unsigned N;
    std::cin >> N;
    unsigned countLong = 0;
    for (auto it = gw1.begin(); it; it.next()) {
        if (it.get().size() > N) ++countLong;
    }
    cout << "   Words with length > N: " << countLong << endl;

    // 6) Все слова заданной длины
    cout << "6) Enter L: ";
    unsigned L;
    std::cin >> L;
    cout << "   Words of length L:" << endl;
    for (auto it = gw1.begin(); it; it.next()) {
        if (it.get().size() == L) cout << it.get() << endl;
    }

    // 7) Количество предложений, где слов > X
    cout << "7) Enter X: ";
    unsigned X;
    std::cin >> X;
    unsigned sentMore = 0;
    for (auto it = gw1.begin(); it; it.skip())
        if (it.sentence().count > X) ++sentMore;
    cout << "   Sentences with more than X words: " << sentMore << endl;

    // 8) Количество слов минимальной длины
    unsigned minLen = 200000;
    for (auto it = gw1.begin(); it; it.next()) minLen = std::min<unsigned>(minLen, it.get().size());

    unsigned minCount = 0;
    for (auto it = gw1.begin(); it; it.next())
        if (it.get().size() == minLen) ++minCount;

    cout << "8) Words of minimal length (" << minLen << "): " << minCount << endl;

    // 9) Слова без цифр и с заглавной буквы
    cout << "9) Words with no digits and starting with uppercase:" << endl;
    for (auto it = gw1.begin(); it; it.next()) {
        const string& w = it.get();
        if (!w.empty() && isupper(w[0])) {
            bool ok = true;
            for (char c : w)
                if (isdigit(c)) ok = false;
            if (ok) cout << w << endl;
        }
    }

    // 10) Слова только из цифр
    cout << "10) Words of digits only:" << endl;
    for (auto it = gw1.begin(); it; it.next()) {
        const string& w = it.get();
        bool dig = !w.empty();
        for (char c : w)
            if (!isdigit(c)) dig = false;
        if (dig) cout << w << endl;
    }

    // 11) Количество слов с заглавной буквы
    unsigned capCount = 0;
    for (auto it = gw1.begin(); it; it.next())
        if (!it.get().empty() && isupper(it.get()[0])) ++capCount;
    cout << "11) Words starting with uppercase: " << capCount << endl;

    // 12) Слова, начинающиеся с цифры
    cout << "12) Words starting with digit:" << endl;
    for (auto it = gw1.begin(); it; it.next()) {
        const string& w = it.get();
        if (!w.empty() && isdigit(w[0])) cout << w << endl;
    }

    // 13) Слова без 'd' и 'D'
    cout << "13) Words without 'd' or 'D':" << endl;
    for (auto it = gw1.begin(); it; it.next()) {
        const string& w = it.get();
        bool ok = true;
        for (char c : w)
            if (c == 'd' || c == 'D') ok = false;
        if (ok) cout << w << endl;
    }

    // 14) Предложения в двух файлах и сравнение
    unsigned sc2 = gw2.sentenceCount();
    cout << "14) Sentences in input1: " << sc1 << endl;
    cout << "    Sentences in input2: " << sc2 << endl;
    cout << "    input1 " << (sc1 == sc2 ? '=' : sc1 > sc2 ? '>' : '<') << " input2" << endl;

    return 0;
}
