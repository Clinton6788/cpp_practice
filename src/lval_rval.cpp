#include <iostream>
#include <string>

// Lvalues vs Rvalues

void inspect(const std::string& value){
    std::cout << value << " Const Ref\n";
}

void inspect(std::string&& value){
    std::cout << value << " Other\n";
}

int main(){
    std::string a = "Named String Variable";
    inspect(a);
    inspect(std::string("Temp String"));
    a = "Moved String";
    inspect(std::move(a));
}