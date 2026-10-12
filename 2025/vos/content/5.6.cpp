#include <algorithm>
#include <cmath>
#include <fstream>
#include <random>
#include <vector>

using namespace std;

struct Point {
    double x, y;
};

struct Circle {
    Point c;
    double r;
};

double dist(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

bool inside(const Circle& cir, const Point& p) { return dist(cir.c, p) <= cir.r + 1e-12; }

Point circleCenter(double bx, double by, double cx, double cy) {
    double b = bx * bx + by * by;
    double c = cx * cx + cy * cy;
    double d = bx * cy - by * cx;
    return {(cy * b - by * c) / (2 * d), (bx * c - cx * b) / (2 * d)};
}

Circle circle2(const Point& a, const Point& b) {
    Point c = {(a.x + b.x) / 2.0, (a.y + b.y) / 2.0};
    return {c, dist(a, b) / 2.0};
}

Circle circle3(const Point& a, const Point& b, const Point& c) {
    Point o = circleCenter(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y);
    o.x += a.x;
    o.y += a.y;
    return {o, dist(o, a)};
}

bool valid(const Circle& cir, const vector<Point>& pts) {
    for (const auto& p : pts)
        if (!inside(cir, p)) return false;
    return true;
}

Circle trivial(vector<Point>& pts) {
    if (pts.empty()) return {{0, 0}, 0};
    if (pts.size() == 1) return {pts[0], 0};
    if (pts.size() == 2) return circle2(pts[0], pts[1]);

    for (int i = 0; i < 3; i++)
        for (int j = i + 1; j < 3; j++) {
            Circle c = circle2(pts[i], pts[j]);
            if (valid(c, pts)) return c;
        }
    return circle3(pts[0], pts[1], pts[2]);
}

Circle welzlRec(vector<Point>& pts, vector<Point>& border, int n) {
    if (n == 0 || border.size() == 3) return trivial(border);

    int idx = rand() % n;
    Point p = pts[idx];
    swap(pts[idx], pts[n - 1]);

    Circle c = welzlRec(pts, border, n - 1);
    if (inside(c, p)) return c;

    border.push_back(p);
    Circle res = welzlRec(pts, border, n - 1);
    border.pop_back();
    return res;
}

Circle welzl(vector<Point>& pts) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(pts.begin(), pts.end(), g);
    vector<Point> border;
    return welzlRec(pts, border, pts.size());
}

int main(int argc, char** argv) {
    if (argc < 3) return 1;

    ifstream in(argv[1]);
    ofstream out(argv[2]);
    if (!in || !out) return 2;

    size_t n;
    if (!(in >> n)) return 3;
    if (n == 0) {
        out << 0;
        return 0;
    }

    vector<Point> pts(n);
    for (size_t i = 0; i < n; i++)
        if (!(in >> pts[i].x >> pts[i].y)) return 3;

    Circle c = welzl(pts);
    out << c.c.x << " " << c.c.y << " " << c.r;
    return 0;
}
