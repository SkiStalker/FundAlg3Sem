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
    return (a + b) * N;
}

template <class T, class T_alloc = std::allocator<T>>
void foo()
{

}

template<class _T, size_t N>
void print_static_array(_T(&arr)[N]) {
    cout << "Arr len " << sizeof(arr) / sizeof(arr[0]) << endl;
}

int main()
{
    cout << add(1, 2) << endl
         << add<double>(1, 2) << endl;

    cout << add1<int, 5>(1, 2) << endl;


    int arr1[5];
    float arr2[5];
    double arr3[6];

    print_static_array<int>(arr1); //error 
    print_static_array(arr2);
    print_static_array(arr3);


    print_static_array(arr1);


    foo<int>();
    foo<int, std::allocator<long long>>();

    return 0;
}
