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

void inspect_resource(const Resource& r){
    std::cout << "Inspecting Resource " << r.num << '\n';
}

void take_resource(std::unique_ptr<Resource> r){
    std::cout << "Taking Resource " << r->num << '\n';
}

int main(){
    auto r1 = std::make_unique<Resource>(1);
    auto r2 = std::make_unique<Resource>(2);

    // r2 = r1; Will Error - Unique Pointer does not allow multiple owners

    r2 = std::move(r1);

    // Demonstrate taking of resource (should destroy after func complete)
    auto r4 = std::make_unique<Resource>(4);
    take_resource(std::move(r4));
    std::cout << "R4 now nullptr: " << (r4 ? "False" : "True") << '\n';

    // Demonstrate Standard Ref
    auto r5 = std::make_unique<Resource>(5);
    inspect_resource(*r5);
    std::cout << "R5 Still Exists: " << (r5 ? "True" : "False") << '\n';


    // Check ownership
    // NOTE: cout has higher precedence than the conditional operator '?'
    //  This means without the conditions in parenthesis, only the var is printed
    std::cout << "R1: " << (r1 ? "True" : "False") << '\n';
    std::cout << "R2: " << (r2 ? "True" : "False") << '\n';

    // Reassign r1
    r1 = std::make_unique<Resource>(3);

}
