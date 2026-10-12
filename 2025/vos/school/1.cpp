#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::max;

int main() {
    int a, b, c, d;
    cin >> a >> b >> c >> d;

    int diff = -a - b - c + max(a, max(b, c)) * 2;
    if (diff < 0) diff = -diff;

    int ans = 0;
    if (diff > d) ans = diff - d;

    cout << ans << endl;
    return 0;
}
