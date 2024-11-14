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

class ArrayInt
{
private:
    int *arr = nullptr;
    int sz;

public:
    explicit ArrayInt(int sz) // :arr(new int[sz]), sz(sz)
    {
        this->arr = new int[sz];
        this->sz = sz;
    }
    ArrayInt(const ArrayInt &other)
    {
        if (this->arr)
        {
            delete[] this->arr;
        }
        this->arr = new int[other.sz];
        for (int i = 0; i < other.sz; ++i)
        {
            this->arr[i] = other.arr[i];
        }
        this->sz = other.sz;
    }

    ArrayInt &operator=(const ArrayInt &other)
    {
        if (this->arr == other.arr)
        {
            return *this;
        }
        if (this->arr)
        {
            delete[] this->arr;
        }
        this->arr = new int[other.sz];
        for (int i = 0; i < other.sz; ++i)
        {
            this->arr[i] = other.arr[i];
        }
        this->sz = other.sz;
        return *this;
    }

    int &operator[](int ind)
    {
        if (ind >= 0 && ind < this->sz)
        {
            return this->arr[ind];
        }
        else
        {
            throw exception();
        }
    }

    const int &operator[](int ind) const
    {
        if (ind >= 0 && ind < this->sz)
        {
            return this->arr[ind];
        }
        else
        {
            throw exception();
        }
    }

    int get_size() const
    {
        return this->sz;
    }
};

class Parent
{
    friend class ChildImproved;

public:
    Parent(int init_a)
    {
        cout << "Arg constr" << endl;
        this->a = init_a;
    }

    ~Parent()
    {
        cout << "Destr" << endl;
    }
    void print_a()
    {
        cout << this->a << endl;
    }
    int a = 1;

private:
    int b;

protected:
    int c;
};

class Child : public Parent
{
    friend class ChildImproved;

public:
    Child(int init_baby_a) : Parent(init_baby_a)
    {
        cout << "Baby arg constr" << endl;
    }

    ~Child()
    {
        cout << "Baby destr" << endl;
    }
};

class ChildImproved
{
private:
    Parent p;
};

void change(int &a)
{
    a = 12;
}

void view_child(const Child &child)
{
    int res = child.a;
}

class IRunnable
{
public:
    virtual void run(int m) = 0;
};


class IWalkable
{
public:
    virtual void walk(int m) = 0;
};

class Cat : public IRunnable, public IWalkable
{
private:
    int age;
    int weight;
    bool gender;
    char color[25];
public:
    void run(int m) override
    {
        cout << "Cat running away " << m << " meters" << endl;
    }

    void walk(int m) override
    {
        cout << "Cat walking away " << m << " meters" << endl;
    }
};

int main()
{
    auto c = new Cat();
    IRunnable *r = c;
    IWalkable *w = c;

    r->run(12);

    w->walk(20);

    delete r;
    return 0;
}