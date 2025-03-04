#include <iostream>

using namespace std;

class Parent
{
public:
    operator float() const
    {
        return 0.0;
    }
};

class Child: public Parent
{
public:
    operator int() const
    {
        return 0;
    }
};

Parent add(Parent p, Parent p1)
{
    cout << "Parent overload" << endl;
    return Parent();
}

Parent* add(Parent* p, Parent* p1)
{
    cout << "Parent* overload" << endl;
    return new Parent();
}

Child* add(Child* p, Child* p1)
{
    cout << "Child* overload" << endl;
    return new Child();
}

int add(int a, int b)
{
    cout << "int overload" << endl;
    return a + b;
}

char add(char a, char b)
{
    cout << "char overload" << endl;
    return a + b;
}

unsigned int add(unsigned int a, unsigned int b)
{
    cout << "unsigned int overload" << endl;
    return a + b;
}

double add(double a, double b)
{
    cout << "double int overload" << endl;
    return a + b;
}

float add(float a, float b)
{
    cout << "float int overload" << endl;
    return a + b;
}

int main()
{

    add(1, 1);

    add((char)1, 1); // 1 аргумент будет расширен до int и будет вызвана перегрузка с int

    add((char)1, (char)1);

    add((short)1, (short)1); // Выбор перегрузки будет сделан в пользу более широкого типа int

    add(1LL, 1); // 1 аргумент будет урезан c long long до размера int и будет вызвана перегрузка с int

    // add(1U, 1); Не скомпилится, нельзя подобрать точный вариант перегрузки, подходит как вариант с int, так и вариант с unsigned int

    add(1UL, 1U);

    add(1ULL, 1U); // 1 аргумент будет урезан до размера unsigned long long и будет вызвана перегрузка с unsigned int

    // add(1ULL, 1ULL); // Не скомпилится, нельзя подобрать точный вариант перегрузки, подходят все варианты с целыми числами

    // add(1.0, 1); Не скомпилится, нельзя подобрать точный вариант перегрузки, подходит как вариант с int, так и вариант с double

    add(1.0, 1.0);

    add(1.0F, 1.0); // 1 аргумент будет расширен до double и будет вызвана перегрузка с double

    add(1.0F, 1.0F);

    // add(Parent(), Child()); Не скомпилится, к родительскому типу можно привести только указатель

    add(Parent(), Parent());


    add(new Parent(), new Child());

    add(Child(), 12);


    add(*(new Child()), 1.0F);

    return 0;
}