#include <iostream>

class Money {
    private:
    int cash;  // в копейках
    int nonCash;

    public:
    // перегрузка с параметрами по умолчанию
    Money(int cash = 0, int nonCash = 0) : cash(cash), nonCash(nonCash) {}

    void setCash(int amount) { cash = amount; }
    void setNonCash(int amount) { nonCash = amount; }

    int getCash() const { return cash; }
    int getNonCash() const { return nonCash; }

    void add(int amount) { cash += amount; }
    void add(int amount, bool isCash) {
        if (isCash)
            cash += amount;
        else
            nonCash += amount;
    }

    // поддержки float только в add
    void add(float amount) { cash += static_cast<int>(amount * 100); }
    void add(float amount, bool isCash) {
        if (isCash)
            cash += static_cast<int>(amount * 100);
        else
            nonCash += static_cast<int>(amount * 100);
    }

    bool spendCash(int amount) {
        if (amount > cash) return false;
        cash -= amount;
        return true;
    }

    bool spendNonCash(int amount) {
        if (amount > nonCash) return false;
        nonCash -= amount;
        return true;
    }
};

int main() {
    Money m(500, 1000);

    // std::cout << "Private: " << m.cash << std::endl;

    m.add(200);
    m.add(50, false);
    m.add(3.75f);

    std::cout << "Cash: " << m.getCash() << std::endl;
    std::cout << "NonCash: " << m.getNonCash() << std::endl;

    return 0;
}
