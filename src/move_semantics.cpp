#include <iostream>
#include <vector>
#include <string>
#include <utility>

void print_vec(
        const std::vector<int>& vec,
        const std::string& name
    ){

    if (vec.size() == 0){
        std::cout << name << " Vector Empty\n";
        return;
    }
    std::cout << "-----" << name << "-----\n";
    for (const int& i : vec){
        std::cout << i << "\n";
    }
}

int main(){
    std::vector<int> original = {1,2,3,4,5};
    std::vector<int> copied = original;
    std::vector<int> moved = std::move(original);
    print_vec(original, static_cast<std::string>("Original"));
    print_vec(copied, static_cast<std::string>("Copied"));
    print_vec(moved, static_cast<std::string>("Moved"));
    original.push_back(6);
    print_vec(original, static_cast<std::string>("Original"));
    print_vec(copied, static_cast<std::string>("Copied"));
    print_vec(moved, static_cast<std::string>("Moved"));
}