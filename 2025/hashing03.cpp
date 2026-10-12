#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::nothrow;

#define uint unsigned

struct Node {
    string value;
    Node* next;
};

class HashSet {
    Node **a;
    uint p;

    uint hash(const string& value) const {
        uint sum = 0;
        for(unsigned char c : value) sum += c;
        return sum % p;
    }

    public:
    HashSet(uint p): p(p){
        if(p == 0) exit(1);
        a = new(nothrow) Node*[p];
        if(!a) exit(2);
        for(uint i = 0; i < p; ++i) a[i] = nullptr;
    }

    ~HashSet(){
        Node *n, *n1;
        for(uint i = 0; i < p; ++i) {
            n = a[i];
            while(n){
                n1 = n;
                n = n->next;
                delete n1;
            }
        }
    }

    void add(const string& value){
        uint h = hash(value);
        Node *n = new(nothrow) Node{value, nullptr};
        if(!n) exit(3);
        if(!a[h]){
            a[h] = n;
            return;
        }
        Node *curr = a[h];
        while(curr->next && curr->value > value) curr = curr->next;
        n->next = curr->next;
        curr->next = n;
    }

    void remove(const string& value){
        uint h = hash(value);
        if(!a[h]) return;
        if(a[h]->value == value){
            Node* del = a[h];
            a[h] = a[h]->next;
            delete del;
            return;
        }
        Node *previous = a[h], *curr = previous->next;
        while(curr && curr->value != value) {
            previous = curr;
            curr = curr->next;
        }
        if(!curr) return;
        previous->next = curr->next;
        delete curr;
    }

    bool contain(const string& value) const {
        uint h = hash(value);
        Node *curr = a[h];
        while(curr && curr->value <= value) {
            if(curr->value == value) return true;
            curr = curr->next;
        }
        return false;
    }

    void print() const {
        cout<<'('<<p<<") {\n";
        Node *n;
        for(uint i = 0; i < p; ++i){
            if(!a[i]) continue;
            cout << ' ' << i << ':';
            n = a[i];
            while(n){
                cout<<' '<<n->value;
                n=n->next;
            }
            cout<<'\n';
        }
        cout<<'}'<<endl;
    }
};

// int main(){
//     uint p;
//     cout << "(P): ";
//     cin >> p;

//     HashSet hs(p);
    
//     while(true){
//         string input;
//         cout << "(In): ";
//         cin >> input;
//         if(input == "exit") break;
//         switch (input[0])
//         {
//         case '+':
//             input.erase(0, 1);
//             hs.add(input);
//             break;
//         case '-':
//             input.erase(0, 1);
//             hs.remove(input);
//             break;
//         case '?':
//             input.erase(0, 1);
//             cout << '\'' << input << '\'' << (hs.contain(input) ? " in list" : " not in list") << endl;
//             break;
//         default:
//             hs.print();
//             break;
//         }
//     }
    

//     return 0;
// }

#include <fstream>
using std::ifstream;

int main(){
    uint p;
    cout << "(P): ";
    cin >> p;

    HashSet hs(p);

    

    ifstream file("ojegov525.txt");
    if(!file.is_open()) return 10;

    string s;
    while(file){
        file >> s;
        hs.add(s);
    }

    hs.print();

    return 0;
}