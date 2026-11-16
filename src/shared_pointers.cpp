#include <iostream>
#include <memory>
#include <utility>

class Resource {
    public:
        int num;

        Resource(int assigned_num) 
            : num(assigned_num)
        {
            std::cout << "Resource " << num << " Acquired\n";
        }

        ~Resource(){
            std::cout << "Resource " << num << " Released\n";
        }
};

int main(){
    auto r1 = std::make_shared<Resource>(1);

    // Can copy with shared pointers
    auto r2 = r1;
    auto r3 = r2;

    std::cout << r1.use_count() << '\n';
    std::cout << r2.use_count() << '\n';
    std::cout << r3.use_count() << '\n';

    r2.reset();

    std::cout << r1.use_count() << '\n';
    std::cout << r2.use_count() << '\n';
    std::cout << r3.use_count() << '\n';

    r1.reset();

    std::cout << r3.use_count() << '\n';

}