#include <iostream>
#include <bitset>
#include <utility>

using namespace std;

class binary_int {
private:
    int value;

public:
    binary_int(int val = 0) : value(val) {}

    binary_int operator-() const {
        return binary_int(~value + 1);
    }

    binary_int& operator++() {
        value += 1;
        return *this;
    }

    binary_int operator++(int) {
        binary_int temp = *this;
        ++(*this);
        return temp;
    }

    binary_int& operator--() {
        value -= 1;
        return *this;
    }

    binary_int operator--(int) {
        binary_int temp = *this;
        --(*this);
        return temp;
    }

    binary_int& operator+=(const binary_int& other) {
        int carry;
        int a = value;
        int b = other.value;
        while (b != 0) {
            carry = a & b;
            a ^= b;
            b = carry << 1;
        }
        value = a;
        return *this;
    }

    binary_int operator+(const binary_int& other) const {
        binary_int temp = *this;
        temp += other;
        return temp;
    }

    binary_int& operator-=(const binary_int& other) {
        *this += -other;
        return *this;
    }

    binary_int operator-(const binary_int& other) const {
        binary_int temp = *this;
        temp -= other;
        return temp;
    }

    binary_int& operator*=(const binary_int& other) {
        int a = value;
        int b = other.value;
        int result = 0;
        while (b != 0) {
            if (b & 1) {
                result += a;
            }
            a <<= 1;
            b >>= 1;
        }
        value = result;
        return *this;
    }

    binary_int operator*(const binary_int& other) const {
        binary_int temp = *this;
        temp *= other;
        return temp;
    }

    binary_int& operator<<=(int n) {
        value <<= n;
        return *this;
    }

    binary_int& operator>>=(int n) {
        value >>= n;
        return *this;
    }

    binary_int operator<<(int n) const {
        binary_int temp = *this;
        temp <<= n;
        return temp;
    }

    binary_int operator>>(int n) const {
        binary_int temp = *this;
        temp >>= n;
        return temp;
    }

    friend ostream& operator<<(ostream& os, const binary_int& bi) {
        os << bitset<32>(bi.value);
        return os;
    }

    pair<binary_int, binary_int> split() const {
        int halfBits = sizeof(int) * 8 / 2;
        int upper = (value >> halfBits) << halfBits;
        int lower = value & ((1 << halfBits) - 1);
        return make_pair(binary_int(upper), binary_int(lower));
    }

    int get_value() const {
        return value;
    }
};

int main() {
    binary_int a(12), b(5);

    cout << "a = " << a << "\n"; 
    cout << "b = " << b << "\n"; 

    binary_int c = a + b;
    cout << "a + b = " << c << "\n";

    binary_int d = a - b;
    cout << "a - b = " << d << "\n";

    binary_int e = a * b;
    cout << "a * b = " << e << "\n";

    ++a;
    cout << "++a = " << a << "\n";

    binary_int f = a++;
    cout << "f = " << f << "\n";
    cout << "a = " << a << "\n"; 

    --b;
    cout << "--b = " << b << "\n";

    binary_int k = b--;
    cout << "k = " << k << "\n";
    cout << "b = " << b << "\n"; 

    binary_int g = a << 2;
    cout << "a << 2 = " << g << "\n";

    binary_int h = b >> 1;
    cout << "b >> 1 = " << h << "\n";

    auto splitResult = a.split();
    cout << "Upper half of a: " << splitResult.first << "\n";
    cout << "Lower half of a: " << splitResult.second << "\n";

    return 0;
}