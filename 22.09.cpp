#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

class Game {
    string title;
    int score;

    public:
    Game(string title = "Noname", int score = 0) : title(title), score(score) {
        cout << "[Game]: " << title << " " << score << endl;
    }

    void set(string name, int score) {
        this->title = name;
        this->score = score;
        cout << "[Game::set]: " << title << " " << score << endl;
    }

    ~Game() { cout << "[~Game]: " << title << endl; }
};

class Player {
    string name;
    Game* games;
    int count;
    int size;

    public:
    Player(string name, int size) : name(name), count(0), size(size) {
        cout << "[Player]: " << name << " " << size << endl;
        games = new Game[size];
    }

    void add(string name, int score) {
        if (count < size) games[count++].set(name, score);
    }

    ~Player() {
        delete[] games;
        cout << "[~Player]: " << name << endl;
    }
};

int main() {
    Player player("Me", 3);
    player.add("Hollow Knight", 12);
    player.add("Minecraft", 550);
    player.add("TABS", 135);
    player.add("Dino", 1000000);
    return 0;
}
