#include <iostream>

using namespace std;

namespace MyNamespace
{
    int cout;
    class ArrayInt
    {
    public:
        void foo()
        {
            ::cout << cout << std::endl; // alert!!!!
        }
    };
}