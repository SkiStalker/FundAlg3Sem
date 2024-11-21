#include <exception>
#include <iostream>

using namespace std;

class my_exception : public exception
{
    const char *msg = "my bad";

public:
    const char *what() const noexcept override
    {
        return this->msg;
    }
};

int foo()
{
    throw 2;
}

int foo1()
{
    throw exception();
}

int foo2()
{
    throw new my_exception();
}

int main()
{
    try
    {
        foo();
    }
    catch (int ex)
    {
        cout << "Catch exception" << endl;
        cout << ex << endl;
    }

    try
    {
        foo1();
    }
    catch (exception ex)
    {
        cout << "Catch exception" << endl;
        cout << ex.what() << endl;
    }

    try
    {
        foo2();
    }
    catch (exception ex)
    {
        cout << "Catch exception" << endl;
        cout << ex.what() << endl;
    }
    catch (exception *ex)
    {
        cout << "Catch exception" << endl;
        cout << ex->what() << endl;
    }
}