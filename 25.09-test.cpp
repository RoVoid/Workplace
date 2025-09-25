#include <iostream>
#include <fstream>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::sqrt;
using std::ifstream;

class Point {
    double x, y;
public:
    Point():x(0), y(0){}
    Point(double x, double y) : x(x), y(y) {}
    double get_x(){ return x; }
    double get_y(){ return y; }
    void set_x(double x){ this->x = x; }
    void set_y(double y){ this->y = y; }
    void show() { cout<<"[Point]: "<<x<<' '<<y<<endl; }
};

class Triangle {
    Point p1, p2, p3;
    bool valid; double perimeter;
public:
    Triangle(): valid(false), perimeter(-1) {}
    Triangle(Point p1, Point p2, Point p3) {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        double a = sqrt((p1.get_x() - p2.get_x()) * (p1.get_x() - p2.get_x()) + (p1.get_y() - p2.get_y()) * (p1.get_y() - p2.get_y()));
        double b = sqrt((p3.get_x() - p2.get_x()) * (p3.get_x() - p2.get_x()) + (p3.get_y() - p2.get_y()) * (p3.get_y() - p2.get_y()));
        double c = sqrt((p1.get_x() - p3.get_x()) * (p1.get_x() - p3.get_x()) + (p1.get_y() - p3.get_y()) * (p1.get_y() - p3.get_y()));
        // cout<<"[Triangle::init]: " << a<<' '<<b<<' '<<c<<endl;
        valid = a + b > c && a + c > b && b + c > a;
        perimeter = valid ? a + b + c : -1;
    }
    void show() {
        cout<<"[Triangle]: " << valid<<' '<<perimeter<<endl;
        cout<<'\t'; p1.show();
        cout<<'\t'; p2.show();
        cout<<'\t'; p3.show();
    }
    bool is_valid() { return valid; }
    double get_perimeter() { return perimeter; }
};

int main() {
    ifstream input("/home/stud-03/Desktop/Test/input.txt");
    if(!input) {
        cout<<"Can't open file!";
        return 1;
    }

    int n;
    if(!(input>>n)) {
        cout<<"Input is invalid!";
        return 2;
    }

    Triangle *triangles = new(std::nothrow) Triangle[n];
    if(!triangles){
        cout<<"Array don't init!";
        return 3;
    }

    double x1, y1, x2, y2, x3, y3;
    for(int i = 0; i < n; ++i) {
        if(!(input>>x1>>y1>>x2>>y2>>x3>>y3)) {
            cout<<"Input is invalid!";
            delete [] triangles;
            return 4;
        }
        triangles[i] = Triangle(Point(x1, y1), Point(x2, y2), Point(x3, y3));
    }

    for(int i = 0; i < n; ++i) {
        cout<<i+1<<endl;
        triangles[i].show();
        if(triangles[i].is_valid()) cout<<'\t'<<triangles[i].get_perimeter()<<endl;
    }

    delete [] triangles;
    return 0;
}
