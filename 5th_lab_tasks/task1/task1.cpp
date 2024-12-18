#include <iostream>
#include <cmath>
#include <bitset>

class binary_int
{
private:
    int _value;
    static int add(int value_1, int value_2)
    {
        int logic_and = value_1 & value_2;

        int keep = logic_and << 1;
        int res = value_1 ^ value_2;
        while (keep != 0)
        {
            logic_and = keep & res;
            res = (keep ^ res);
            keep = logic_and << 1;
        }
        return res;
    }

    int static negation(int value)
    {
        return add(~value, 1);
    }

public:
    binary_int(int value = 0) : _value(value) {}
    binary_int operator-() { return binary_int(negation(_value)); }
    binary_int operator+(binary_int const &bi) const
    {
        int new_value = add(_value, bi._value);
        return binary_int(new_value);
    }

    binary_int &operator+=(binary_int const &bi)
    {
        _value = add(_value, bi._value);
        return *this;
    }

    binary_int operator-(binary_int const &bi) const
    {
        int new_value = add(_value, negation(bi._value));
        return binary_int(new_value);
    }

    binary_int &operator-=(binary_int const &bi)
    {
        _value = add(_value, negation(bi._value));
        return *this;
    }

    binary_int operator*(binary_int const &bi) const
    {
        int res = 0, a = _value, b = bi._value;
        if (b < 0)
        {
            a = negation(a);
            b = negation(b);
        }
        while (b != 0)
        {
            if (b & 1)
            {
                res = add(res, a);
            }
            a <<= 1;
            b >>= 1;
        }
        return binary_int(res);
    }

    binary_int operator*=(binary_int const &bi)
    {
        int res = 0, a = _value, b = bi._value;
        if (b < 0)
        {
            a = negation(a);
            b = negation(b);
        }
        while (b != 0)
        {
            if (b & 1)
            {
                res = add(res, a);
            }
            a <<= 1;
            b >>= 1;
        }
        _value = res;
        return *this;
    }

    binary_int &operator++()
    {
        _value = add(_value, 1);
        return *this;
    }
    binary_int operator++(int)
    {
        binary_int tmp(*this);
        _value = add(_value, 1); // ++*this;
        return tmp;
    }

    binary_int &operator--()
    {
        _value = add(_value, negation(1));
        return *this;
    }
    binary_int operator--(int)
    {
        binary_int tmp(_value);
        _value = add(_value, negation(1));
        return tmp;
    }

    binary_int operator>>(int shift) const { return binary_int(_value >> shift); }
    binary_int operator<<(int shift) const { return binary_int(_value << shift); }

    binary_int &operator>>=(int shift)
    {
        _value >>= shift;
        return *this;
    }
    binary_int &operator<<=(int shift)
    {
        _value <<= shift;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, binary_int &bi)
    {
        out << std::bitset<sizeof(bi._value) << 3>(bi._value);
        return out;
    }

    int value_accessor() const { return _value; }

    std::pair<binary_int, binary_int> get_pair() const
    {
        short shift = sizeof(_value) << 2;
        std::pair<binary_int, binary_int> p;
        p.first = binary_int(_value & (negation(1) >> shift) << shift);
        p.second = binary_int(_value & (add((1 << shift), negation(1))));
        return p;
    }

public:
    virtual ~binary_int() = default;
};

int main()
{
    binary_int a(1223), b, c;
    std::pair<binary_int, binary_int> p;

    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;

    a = -a;
    std::cout << "___Test (a = -a)___:" << std::endl;
    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;

    b = a++;
    std::cout << "___Test (b = a++)___:" << std::endl;
    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;

    b = ++a;
    std::cout << "___Test (b = ++a)___:" << std::endl;
    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;

    b = a--;
    std::cout << "___Test (b = a--)___:" << std::endl;
    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;

    b = --a;
    std::cout << "___Test (b = a--)___:" << std::endl;
    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;

    b += a;
    std::cout << "___Test (b += a)___:" << std::endl;
    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;

    b -= a;
    std::cout << "___Test (b -= a)___:" << std::endl;
    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;

    b *= a;
    std::cout << "___Test (b *= a)___:" << std::endl;
    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;

    c = a + b;
    std::cout << "___Test (c = a + b)___:" << std::endl;
    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;
    std::cout << "c = " << c.value_accessor() << "; bits: " << c << std::endl;

    c = a - b;
    std::cout << "___Test (c = a - b)___:" << std::endl;
    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;
    std::cout << "c = " << c.value_accessor() << "; bits: " << c << std::endl;

    c = a * b;
    std::cout << "___Test (c = a * b)___:" << std::endl;
    std::cout << "a = " << a.value_accessor() << "; bits: " << a << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;
    std::cout << "c = " << c.value_accessor() << "; bits: " << c << std::endl;

    std::cout << "___Test (b <<= 2)___:" << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;
    b <<= 2;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;

    std::cout << "___Test (b >>= 5)___:" << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;
    b >>= 5;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;

    std::cout << "___Test (b = b >> 2)___:" << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;
    b = b >> 2;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;

    std::cout << "___Test (b = b << 7)___:" << std::endl;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;
    b = b << 7;
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;

    std::cout << "___Test (b.get_pair())___:" << std::endl;
    p = b.get_pair();
    std::cout << "b = " << b.value_accessor() << "; bits: " << b << std::endl;
    std::cout << "Pair b:\n"
              << p.first << '\n'
              << p.second << std::endl;

    return 0;
}