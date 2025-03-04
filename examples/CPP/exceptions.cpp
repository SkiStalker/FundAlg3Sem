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
    throw 2; // bad practice
}

int foo1()
{
    throw my_exception();
}

int foo2()
{
    throw new my_exception(); // unused after c++17
}

int main()
{
    try
    {
        /// exception zone
        /////////////////
        foo();
        ////////////////
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
    catch (my_exception ex)
    {
        cout << "My catch exception" << endl;
        cout << ex.what() << endl;
    }
    catch (exception ex)
    {
        cout << "Catch exception" << endl;
        cout << ex.what() << endl;
    }
    catch (...)
    {
        cout << "How we have passed here..." << endl;
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