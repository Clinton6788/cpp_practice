#include <iostream>
#include <memory>
/*
- This is introduced to avoid the following hard ref memory leak:

class A {
public:
    std::shared_ptr<B> b;
};

class B {
public:
    std::shared_ptr<A> a;
};

- Since both are referenced by eachother, neither can ever be released.
- Instead, prefer:

class A {
public:
    std::shared_ptr<B> b;
};

class B {
public:
    std::weak_ptr<A> a;
};

*/
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

    std::cout << r1.use_count() << '\n'; //2 - shared ptr

    std::weak_ptr<Resource> weak = r1;

    std::cout << r1.use_count()<< '\n'; // 2 - weak ptr doesn't count

    if (auto locked = weak.lock()) {
        std::cout << locked->num << '\n';
        std::cout << r1.use_count() << '\n'; // 3 - Weak ptr now counts
    }

    r1.reset();

    if (auto locked = weak.lock()) {
        std::cout << locked->num<< '\n';
    }
    r3.reset();

    if (auto locked = weak.lock()) {
        std::cout << locked->num<< '\n';
    }


}