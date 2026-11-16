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
    auto r3 = r1;

    std::cout << r1.use_count() << '\n';

    std::weak_ptr<Resource> weak = r1;

    std::cout << r1.use_count(); // 2

    if (auto locked = weak.lock()) {
        std::cout << locked->num;
    }

    r1.reset();

    if (auto locked = weak.lock()) {
        std::cout << locked->num;
    }
    r3.reset();

    if (auto locked = weak.lock()) {
        std::cout << locked->num;
    }


}