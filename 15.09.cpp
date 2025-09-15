#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int var = 0;

namespace test {
    int copy(const char* f1, const char* f2) {
        ifstream fin(f1);
        ofstream fout(f2);
        if (!fin.is_open() || !fout.is_open()) {
            cerr << "Error open files!" << endl;
            return 1;
        }

        string fields[3];
        short numbers[2];

        const char* msgs[5] = {"Surname", "Name", "Father", "Year", "Score"};

        for (int i = 0; i < 3; ++i) {
            if (!(fin >> fields[i])) {
                cerr << "Fail " << msgs[i] << "!" << endl;
                return 1;
            }
            fout << fields[i] << endl;
        }

        for (int i = 0; i < 2; ++i) {
            if (!(fin >> numbers[i])) {
                cerr << "Fail " << msgs[i + 3] << "!" << endl;
                return 1;
            }
            fout << numbers[i] << endl;
        }

        if (fin >> ws && !fin.eof()) {
            cerr << "Too many data!" << endl;
            return 1;
        }

        fin.close();
        fout.close();

        return 0;
    };
}  // namespace test

int main(int argc, char** argv) {
    int var = 1;

    const char* f1 = "1.txt";
    const char* f2 = "2.txt";

    if (argc >= 3) {
        f1 = argv[1];
        f2 = argv[2];
    }

    cout << "Global var = " << ::var << endl;
    cout << "Local var = " << var << endl;

    return test::copy(f1, f2);
}
