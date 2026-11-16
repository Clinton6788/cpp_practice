#include <iostream>
#include <string>
#include <vector>
#include <utility>

class Player {
private:
    std::string name;
    std::vector<int> scores;

public:
    Player(const std::string& name)
        : name(name) {}

    // Copy operations
    Player(const Player&) = default;
    Player& operator=(const Player&) = default;

    // Move constructor
    Player(Player&& other) noexcept
        : name(std::move(other.name)),
        scores(std::move(other.scores)) {}

    // Move Assignment
    Player& operator=(Player&& other) noexcept {
        name = std::move(other.name);
        scores = std::move(other.scores);

        return *this;
    }

    void add_score(int score) {
        scores.push_back(score);
    }


    void print() const {
        std::cout << name << ": ";

        for (const int& score : scores) {
            std::cout << score << " ";
        }

        std::cout << '\n';
    }
};

int main() {
    Player p1("Jeff");

    p1.add_score(10);
    p1.add_score(20);
    p1.add_score(30);

    // Move Constructor
    Player p2 = std::move(p1);

    // Move Assignment
    Player p3("Clint");
    p3.add_score(15);
    // Actual Move
    p3 = std::move(p1);
    // Add Values
    p3.add_score(45);
    p1.add_score(11);



    p2.print();
    p1.print();
    p3.print();
}