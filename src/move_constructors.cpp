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

    Player p2 = std::move(p1);

    p2.print();
    p1.print();
}