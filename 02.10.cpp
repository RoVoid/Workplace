#include <iostream>

class pair{
    int a, b;
    public:
    pair(int a, int b): a(a), b(b){}
    friend bool isFactor(const pair&);
};

class truck;

class car {
    int speed;
    public:
    car(int speed): speed(speed){}
    int speedDiff(const truck&);
    friend int speedDiff(const car&, const truck&);
};

class truck {
    int speed;
    public:
    truck(int speed): speed(speed){}
    friend int car::speedDiff(const truck&);
    friend int speedDiff(const car&, const truck&);
};

bool isFactor(const pair& p){
    return p.a % p.b == 0;
}

int speedDiff(const car& c, const truck& t){
    return c.speed - t.speed;
}

int car::speedDiff(const truck& t){
    return speed - t.speed;
}

int main(){
    pair p1(5, 3), p2(12, 4);
    std::cout<<isFactor(p1)<<' '<<isFactor(p2)<<std::endl;

    car c(80); truck t(50);
    std::cout<<speedDiff(c, t)<<' '<<c.speedDiff(t)<<std::endl;

    return 0;
}