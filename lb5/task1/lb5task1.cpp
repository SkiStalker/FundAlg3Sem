#include <iostream>
#include <bitset> 
using namespace std;

class binary_int {
private:
    int value; 


    void printBinary() const {
        cout << bitset<32>(value);
    }

public:

    binary_int(int v = 0) : value(v) {}

   
    binary_int operator-() const {
        return binary_int(add(~value, 1));
    }


    binary_int& operator++() {
        value = add(value, 1); 
        return *this;
    }


    binary_int operator++(int) {
        binary_int temp = *this;
        ++(*this); 
        return temp;
    }

    
    binary_int& operator--() {
        value = subtract(value, 1); 
        return *this;
    }

 
    binary_int operator--(int) {
        binary_int temp = *this;
        --(*this); 
        return temp;
    }


    binary_int& operator+=(const binary_int& other) {
        value = add(value, other.value);
        return *this;
    }


    binary_int operator+(const binary_int& other) const {
        binary_int result = *this;
        result += other; 
        return result;
    }

   
    binary_int& operator-=(const binary_int& other) {
        value = subtract(value, other.value); 
        return *this;
    }


    binary_int operator-(const binary_int& other) const {
        binary_int result = *this;
        result -= other; 
        return result;
    }


    binary_int& operator*=(const binary_int& other) {
        value = multiply(value, other.value); 
        return *this;
    }

  
    binary_int operator*(const binary_int& other) const {
        binary_int result = *this;
        result *= other; 
        return result;
    }


    binary_int& operator<<=(int shift) {
        value = value << shift; 
        return *this;
    }


    binary_int& operator>>=(int shift) {
        value = value >> shift; 
        return *this;
    }


    binary_int operator<<(int shift) const {
        binary_int result = *this;
        result <<= shift;
        return result;
    }


    binary_int operator>>(int shift) const {
        binary_int result = *this;
        result >>= shift;
        return result;
    }


    friend ostream& operator<<(ostream& os, const binary_int& obj) {
        obj.printBinary();
        return os;
    }


    pair<binary_int, binary_int> splitBits() const {
        int high = (value & 0xFFFF0000) >> 16; 
        int low = value & 0x0000FFFF; 
        return { binary_int(high), binary_int(low) };
    }

private:
    static int add(int a, int b) {
        while (b != 0) {
            int carry = a & b;  
            a = a ^ b;          
            b = carry << 1;     
        }
        return a;
    }
    

    static int subtract(int a, int b) {
        return add(a, add(~b, 1)); 
    }


    static int multiply(int a, int b) {
        int result = 0;
        while (b != 0) {
            if (b & 1) {             
                result = add(result, a);
            }
            a <<= 1;                 
            b >>= 1;                 
        }
        return result;
    }
};

int main() {
    binary_int a(5), b(3);

    cout << "a: " << a << endl;
    cout << "b: " << b << endl;

    binary_int sum = a + b;
    cout << "Sum: " << sum << endl;

    binary_int product = a * b;
    cout << "Product: " << product << endl;

    binary_int diff = a - b;
    cout << "Difference: " << diff << endl;

    cout << "Postfix increment of a: " << a++ << endl;
    cout << "New value of a: " << a << endl;

    auto [high, low] = a.splitBits();
    cout << "High bits: " << high << ", Low bits: " << low << endl;

    return 0;
}
