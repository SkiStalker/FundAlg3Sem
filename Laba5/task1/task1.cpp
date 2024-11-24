#include <iostream>
#include <bitset>
#include <climits>

using namespace std;

class binary_int {
private:
    int value;

    string to_binary() const {
        return bitset<sizeof(int) * CHAR_BIT>(value).to_string();
    }

public:
    binary_int() : value(0) {}
    binary_int(int v) : value(v) {}

    binary_int operator-() const {
        return binary_int(~value + 1);
    }

    binary_int& operator++() {
        int carry = 1;
        while (carry) {
            int temp = value & carry;
            value ^= carry;
            carry = temp << 1;
        }
        return *this;
    }

    binary_int operator++(int) {
        binary_int temp(*this);
        ++(*this);
        return temp;
    }

    binary_int& operator--() {
        int borrow = 1;
        while (borrow) {
            int temp = (~value) & borrow;
            value ^= borrow;
            borrow = temp << 1;
        }
        return *this;
    }

    binary_int operator--(int) {
        binary_int temp(*this);
        --(*this);
        return temp;
    }

    binary_int& operator+=(const binary_int& other) {
        int carry = 0;
        int b = other.value;
        while (b) {
            carry = value & b;
            value ^= b;
            b = carry << 1;
        }
        return *this;
    }

    binary_int operator+(const binary_int& other) const {
        binary_int result(*this);
        result += other;
        return result;
    }

    binary_int& operator-=(const binary_int& other) {
        *this += -other;
        return *this;
    }

    binary_int operator-(const binary_int& other) const {
        binary_int result(*this);
        result -= other;
        return result;
    }

    binary_int& operator*=(const binary_int& other) {
        binary_int result;
        binary_int multiplier = *this;
        int b = other.value;
        while (b) {
            if (b & 1) {
                result += multiplier;
            }
            multiplier.value <<= 1;
            b >>= 1;
        }
        value = result.value;
        return *this;
    }

    binary_int operator*(const binary_int& other) const {
        binary_int result(*this);
        result *= other;
        return result;
    }

    binary_int& operator<<=(int shift) {
        value <<= shift;
        return *this;
    }

    binary_int operator<<(int shift) const {
        binary_int result(*this);
        result <<= shift;
        return result;
    }

    binary_int& operator>>=(int shift) {
        value >>= shift;
        return *this;
    }

    binary_int operator>>(int shift) const {
        binary_int result(*this);
        result >>= shift;
        return result;
    }

    friend ostream& operator<<(ostream& os, const binary_int& obj) {
        os << obj.to_binary();
        return os;
    }

    pair<binary_int, binary_int> split_half() const {
        int num_bits = sizeof(int) * CHAR_BIT;
        int half_bits = num_bits / 2;

        int high_mask = ((1 << half_bits) - 1) << half_bits;
        int low_mask = (1 << half_bits) - 1;

        binary_int high_part((value & high_mask));
        binary_int low_part((value & low_mask));

        return {high_part, low_part};
    }

    int get_value() const { return value; }
};

int main() {
    binary_int a(2), b(1);

    cout << "a: " << a << "\n";
    cout << "b: " << b << "\n";
    cout << "a + b: " << (a + b) << "\n";
    cout << "a - b: " << (a - b) << "\n";
    ++a;
    cout << "++a: " << a << "\n";
    b--;
    cout << "b--: " << b << "\n";
    a *= b;
    cout << "a *= b: " << a << "\n";
    cout << "a << 2: " << (a << 2) << "\n";
    cout << "b >> 1: " << (b >> 1) << "\n";
    auto [high, low] = a.split_half();
    cout << "High half: " << high << "\n";
    cout << "Low half: " << low << "\n";
    return 0;
}
