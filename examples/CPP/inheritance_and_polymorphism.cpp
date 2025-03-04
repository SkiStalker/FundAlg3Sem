#include <iostream>

class Parent
{
public:
    Parent()
    {
        std::cout << "Parent constr" << std::endl;
    }

    ~Parent()
    {
        std::cout << "Parent destr" << std::endl;
    }
};

class Child : public Parent
{
public:
    Child()
    {
        std::cout << "Child constr" << std::endl;
    }

    ~Child()
    {
        std::cout << "Child destr" << std::endl;
    }
};

class VParent
{
public:
    VParent()
    {
        std::cout << "VParent constr" << std::endl;
    }
    virtual void foo()
    {
        std::cout << "VParent foo" << std::endl;
    }
    virtual ~VParent()
    {
        std::cout << "VParent destr" << std::endl;
    }
};

class VChild : public VParent
{
public:
    VChild()
    {
        std::cout << "VChild constr" << std::endl;
    }

    virtual void foo() override final
    {
        std::cout << "VChild foo" << std::endl;
    }

    virtual ~VChild()
    {
        std::cout << "VChild destr" << std::endl;
    }
};

class Grandson final : public VChild
{
public:
    Grandson()
    {
        std::cout << "Grandson constr" << std::endl;
    }

    // void foo() // Не скомпилится, так как в родительском классе функция объявлена final 
    // {
    //     std::cout << "VChild foo" << std::endl;
    // }

    ~Grandson()
    {
        std::cout << "Grandson destr" << std::endl;
    }
};

// class GrandGrandson: public Grandson {}; // Не скомпилится, так как родительский класс объявлен final

int main()
{
    {
        std::cout << "Create as Child* destroy as Parent*:" << std::endl;
        Parent *c = new Child();
        delete c;
        std::cout << "We have no Child destr" << std::endl << std::endl;
    }

    {
        std::cout << "Create as VChild* destroy as VParent*:" << std::endl;
        VParent *c = new VChild();
        delete c;
        std::cout << "OK we have VChild destr" << std::endl << std::endl;
    }

    {
        std::cout << "Create as Grandson* destroy as VParent*:" << std::endl;
        VParent *c = new Grandson();
        delete c;
        std::cout << "Even without explicit virtual modifier in VChild destructor, it keep virtual for Grandson" << std::endl << std::endl;
    }
    std::cout << "A destructor is a special example of polymorphism. For other methods of the class, it works the same way" << std::endl;
}
