#include <exception>

using namespace std;

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

int main()
{
    ArrayInt b{5};

    return 0;
}