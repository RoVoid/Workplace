#include <algorithm>
#include <vector>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::swap;
using std::vector;

int main() {
    int n, a, b, x, y;
    cin >> n >> a >> x >> b >> y;

    if (x < y) {
        swap(a, b);
        swap(x, y);
    }

    vector<int> indexs(n*2, 0);

    for (int i = x; i < n;) {
        if (a > 0) {
            indexs.push_back(i);
            indexs.push_back(x);
            --a;
            i += 2 * x + 1;
            if (a == 0 && b > 0) i -= x - y;
        } else if (b > 0) {
            indexs.push_back(i);
            indexs.push_back(y);
            --b;
            i += 2 * y + 1;
        } else {
            cout << -1 << endl;
            return 0;
        }
    }

    for(int i = 0; i < indexs.size(); i+=2){
        cout << indexs[i] << ' ' << indexs[i+1] << endl;
    }

    return 0;
}
