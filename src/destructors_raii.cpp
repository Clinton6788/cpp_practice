#include <iostream>

class Resource {
    public:
        int num;

        Resource(int assigned_num) 
            : num(assigned_num)
        {
            std::cout << "Resource" << num << "Acquired\n";
        }

        ~Resource(){
            std::cout << "Resource" << num << "Released\n";
        }
};

int main(){
    // Acquires: 1,2,3,4
    // Releases: 4,3,2
    // Acquires: 5
    // Releases: 5,1
    Resource r1(1);
    {
        Resource r2(2);
        Resource r3(3);
        {
            Resource r4(4);
        }
    }
    Resource r5(5);
}