#include <cstdint>
#include <iostream>

template <class T>
class MyClass {
   public:
	MyClass(std::size_t sz, T fill_value) {
        std::cout << "std::size_t Regular constr" << std::endl;
    }


    // Если бы этот кусок кода был бы, то мы бы не получили такого поведения 
    // 
    // MyClass(T sz, T fill_value) {
    //     std::cout << "int Regular constr" << std::endl;
    // }
    //

    template<class InIter>
	MyClass(InIter start, InIter end) {
        std::cout << "Iter constr" << std::endl;
    }
};


int main() {

    MyClass<int> a(12, 5);


    
    MyClass<int> b(12UL, 5);
    
    return 0;
}