#include <string>
#include <vector>
#include <iostream>

class Player{
private:
    std::string name;
    std::vector<int> scores;

public:
    Player(const std::string& name)
        : name(name){}

    void add_score(int score) {
        scores.push_back(score);
    }

    int get_total() const {
        int t = 0;
        for (const int& s : scores){
            t += s;
        }
        return t;
    }

    double get_average() const {
        if (scores.size() == 0){
            return 0;
        }
        return static_cast<double>(this->get_total()) / scores.size();
    }

    const int* get_highest() const {
        const int* h = nullptr;

        if (scores.size() == 0){
            return h;
        }

        for (const int& s : scores){
            if (!h || *h < s){
                h = &s;
            }
        }
        return h;
    }

    const std::string& get_name() const {
        return name; // This SHOULD be valid -> return type auto-converts to const ref??
    }

    void print_player() const {
        std::cout << "\n------------------\n";
        std::cout << "\nName: " << name;
        std::cout << "\nTotal: " << this->get_total();
        std::cout << "\nAverage: " << this->get_average();
        const int* highest = get_highest();

        if (highest) {
            std::cout << "\nHighest: " << *highest;
        }
        else {
            std::cout << "\nHighest: N/A";
        }    
    }
};

void copy_semantics() {
    Player p1 = Player("Player One");
    Player p2 = p1;
    Player& p3 = p1;

    p2.add_score(56);
    p3.add_score(72);
    p1.print_player();
    p2.print_player();
    p3.print_player();
}

int main() {
    Player p1(static_cast<std::string>("Jeff"));
    Player p2(static_cast<std::string>("George"));

    for (int i = 1; i < 10; i++) {
        if (i % 2 == 0){
            p1.add_score(i*2);
        }
        else {
            p2.add_score(i*2);
        }
    }
    p1.print_player();
    p2.print_player();
    std::cout << "\n\n----- Copy Semantics -----\n";
    copy_semantics();
}
