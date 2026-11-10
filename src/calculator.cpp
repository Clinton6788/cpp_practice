#include <iostream>

int add(const int& x, const int& y);
int subtract(const int& x, const int& y);
int multiply(const int& x, const int& y);
float divide(const int& x, const int& y);

// A few improvements:
// - Prefer copies of smaller data (ints, floats) rather than references
//      - Case by case


int main(void) {
    int x;
    int y;

    // Prompt
    std::cout << "Enter First Number: \n";
    std::cin >> x;
    std::cout << "\nEnter Second Number: \n";
    std::cin >> y;

    // Ack
    std::cout << "\nYou entered: " << x << " and " << y << "\n";
    
    // Results
    const auto sum = add(x,y);
    const auto difference = subtract(x,y);
    const auto product = multiply(x,y);
    const auto quotient = divide(x,y);

    // Display
    std::cout << "Sum: " << sum;
    std::cout << "\nDifference: " << difference;
    std::cout << "\nProduct: " << product;
    std::cout << "\nQuotient: " << quotient;
}

int add(const int& x, const int& y) {
    return x + y;
}

int subtract(const int& x, const int& y){
    return x - y;
}

int multiply(const int& x, const int& y){
    return x * y;
}

float divide(const int& x, const int& y){
    if (y == 0) {
        return 0.0;
    }
    return static_cast<float>(x) / y;
}