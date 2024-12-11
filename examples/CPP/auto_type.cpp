#include <iostream>

using namespace std;

int a = 12;

const int &foo3()
{
    cout << "foo3" << endl;
    return a;
}

int main()
{
    decltype(auto) c = foo3();
    auto d = foo3();
    return 0;
}