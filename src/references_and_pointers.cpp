#include <iostream>

int main(){
    // Init two Values
    int a = 10;
    int b = 20;

    // Print Initial Values
    std::cout << "Initial A: " << a << '\n';
    std::cout << "Initial B: " << b << '\n';

    // Ref A, Pointer B
    int& mut_a_ref = a;
    int* mut_b_ptr = &b;

    // Const Ref B, Const A ptr, Const Ptr B
    const int& imm_b_ref = b;
    const int* imm_a_ptr = &a;
    int* const const_b_ptr = &b;

    // Modify A through ref
    mut_a_ref = 50;

    // Modify B through pointer
    *mut_b_ptr = 100;

    // Print Current Values
    std::cout << "Present A: " << mut_a_ref << '\n';
    std::cout << "Present B: " << *mut_b_ptr << '\n';

    // Redirect B ptr to point at A
    mut_b_ptr = &a;

    // Erroneous modify of const pointer
    // *imm_a_ptr = 10; // Fails

    // Erroneous pointer change
    // const_b_ptr = &a; // Error

    // Print All Final Values
    std::cout << "Final A: " << a << '\n';
    std::cout << "Final B: " << b << '\n';
    std::cout << "Const B Ref: " << imm_b_ref << '\n';
    std::cout << "Const A Ptr: " << *imm_a_ptr << '\n';
    std::cout << "Const Ptr to B: " << *const_b_ptr << '\n';
    std::cout << "B ptr changed to A: " << *mut_b_ptr << '\n';
}