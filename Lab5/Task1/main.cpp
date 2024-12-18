#include <iostream>
#include <bitset>
#include <utility>
#include <limits>
#include <string>
#include <cstdint>

using namespace std;

class binary_int
{
private:
    int value;

    static int ultimateSum(int a, int b)
    {
        while (b != 0)
        {
            int carry = a & b;
            a = a ^ b;
            b = carry << 1;
        }
        return a;
    }

    static int ultimateSubtract(int a, int b)
    {
        while (b != 0)
        {
            int borrow = (~a) & b;
            a = a ^ b;
            b = borrow << 1;
        }
        return a;
    }

    static int ultimateMultiply(int a, int b)
    {
        int result = 0;
        while (b != 0)
        {
            if (b & 1)
            {
                result = ultimateSum(result, a);
            }
            a <<= 1;
            b >>= 1;
        }
        return result;
    }

public:
    binary_int(int val = 0) : value(val) {}

    binary_int operator-() const
    {
        return binary_int(ultimateSum(~value, 1));
    }

    binary_int operator++()
    {
        value = ultimateSum(1, value);
        return *this;
    }

    binary_int operator++(int)
    {
        int tmp = value;
        value = ultimateSum(value, 1);
        return tmp;
    }

    binary_int operator--()
    {
        value = ultimateSubtract(value, 1);
        return *this;
    }

    binary_int operator--(int)
    {
        int tmp = value;
        value = ultimateSubtract(value, 1);
        return tmp;
    }

    binary_int &operator+=(const binary_int &other)
    {
        value = ultimateSum(value, other.value);
        return *this;
    }

    binary_int operator+(const binary_int &other) const
    {
        binary_int tmp = (*this);
        tmp += other;
        return tmp;
    }

    binary_int &operator-=(const binary_int &other)
    {
        value = ultimateSubtract(value, other.value);
        return *this;
    }

    binary_int operator-(const binary_int &other) const
    {
        return binary_int(ultimateSubtract(value, other.value));
    }

    binary_int &operator*=(const binary_int &other)
    {
        value = ultimateMultiply(value, other.value);
        return *this;
    }

    binary_int operator*(const binary_int &other) const
    {
        return binary_int(ultimateMultiply(value, other.value));
    }

    binary_int &operator<<=(int shift)
    {
        value <<= shift;
        return *this;
    }

    binary_int &operator>>=(int shift)
    {
        value >>= shift;
        return *this;
    }

    binary_int operator<<(int shift) const
    {
        return binary_int(value << shift);
    }

    binary_int operator>>(int shift) const
    {
        return binary_int(value >> shift);
    }

    friend ostream &operator<<(ostream &os, const binary_int &bin)
    {
        const size_t NBITS = numeric_limits<unsigned int>::digits;
        const bitset<NBITS> bits(bin.value);

        const string strBits = bits.to_string();

        const auto firstDigit = strBits.find('1');

        if (firstDigit != string::npos)
        {
            cout << strBits.substr(firstDigit);
        }
        else
        {
            cout << "0";
        }

        return os;
    }

    pair<binary_int, binary_int> split() const
    {
        const int half_bits = sizeof(value) >> 1;

        uint32_t lower_mask = ultimateSubtract((1 << half_bits), 1);

        binary_int lower_half(value & lower_mask);

        binary_int upper_half((value >> half_bits) & lower_mask);

        return make_pair(upper_half, lower_half);
    }

    int getVal() const
    {
        return value;
    }
};

int main()
{
    binary_int a(10);
    binary_int b(5);
    binary_int g(3);

    cout << a << endl;
    cout << a-- << endl;
    cout << a << endl;

    cout << "a: " << a << endl; // 1010
    cout << "b: " << b << endl; // 101

    binary_int c = a + b + g;
    cout << "a + b: " << c << endl; // 1111

    c += binary_int(3);
    cout << "c += 3: " << c << endl; // 10010

    binary_int d = c - b;
    cout << "c - b: " << d << endl; // 1101

    d *= binary_int(3);
    cout << "d *= 3: " << d << endl; // 100111

    d <<= 1;
    cout << "d <<= 1: " << d << endl; // 1001110

    d >>= 2;
    cout << "d >>= 2: " << d << endl; // 10011

    binary_int f(8);

    auto split_result = f.split();
    cout << "Split f: " << split_result.first << " | " << split_result.second << endl; // 10 | 0

    binary_int e(1);

    cout << "e++: " << e++ << endl; // 10

    cout << "++e: " << ++e << endl; // 11

    return 0;
}