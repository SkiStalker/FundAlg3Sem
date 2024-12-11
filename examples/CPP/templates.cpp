#include <iostream>

using namespace std;

template <class T>
T add(T a, T b)
{
    return a + b;
}

template <class T, size_t N>
T add1(T a, T b)
{
    return (a + b) * 5;
}

template <class T, class T_alloc = std::allocator<T>>
void foo()
{
}

int main()
{
    cout << add(1, 2) << endl
         << add<double>(1, 2) << endl;

    cout << add1<int, 5>(1, 2) << endl;

    return 0;
}
