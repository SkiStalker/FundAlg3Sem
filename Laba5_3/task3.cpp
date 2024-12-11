#include <iostream>
#include <cstring>

using namespace std;

enum ret_type_t {
    SUCCESS,    //Happy end
    ERROR_MALLOC,   //Failed to allocate memory
};

void logError(int code) {
    switch(code) {
        case ERROR_MALLOC: {
            cout << "Failed to allocate memory" << endl;
        }
    }
}

unsigned int ultimateInc(unsigned int n) {
    unsigned int mask = 1;
    while (n & mask) {
        n ^= mask;
        mask <<= 1;
    }
    n ^= mask;
    return n;
}

unsigned int ultimateDec(unsigned int n) {
    unsigned int mask = 1;
    while (!(n & mask)) {
        n ^= mask;
        mask <<= 1;
    }
    n ^= mask;
    return n;
}

class logical_values_array {
private:
    unsigned int value;

public:
    logical_values_array(unsigned int val = 0) : value(val) {}

    unsigned int get_value() const {
        return value;
    }

    void set_value(unsigned int val) {
        value = val;
    }

    logical_values_array operator~() const {
        return logical_values_array(~value);
    }

    logical_values_array operator&(const logical_values_array& other) const {
        return logical_values_array(value & other.value);
    }

    logical_values_array operator|(const logical_values_array& other) const {
        return logical_values_array(value | other.value);
    }

    //(A -> B) = ~A | B
    logical_values_array implication(const logical_values_array& other) const {
        return ~(*this) | other;
    }

    //(A <-> B) = ~(A ^ B)
    logical_values_array coimplication(const logical_values_array& other) const {
        return ~(*this ^ other);
    }

    //(A ^ B) = A & ~B | ~A & B
    logical_values_array operator^(const logical_values_array& other) const {
        return logical_values_array((value & ~other.value) | (~value & other.value));
    }

    //(A == B) = ~(A ^ B)
    logical_values_array equalant(const logical_values_array& other) const {
        return ~(*this ^ other);
    }

    //(A ↓ B) = ~(A | B)
    logical_values_array pearce(const logical_values_array& other) const {
        return ~(*this | other);
    }

    //(A | B) = ~(A & B)
    logical_values_array sheffer(const logical_values_array& other) const {
        return ~(*this & other);
    }

    static bool equals(const logical_values_array& a, const logical_values_array& b) {
        return a.value == b.value;
    }

    bool get_bit(int position) const {
        return (value >> position) & 1;
    }

    int to_binary_string(char* destiny) {
        unsigned int number = value, memo = sizeof(char) << 2, index = 0;

        while (number > 0) {
            if (index >= memo) {
                memo <<= 1;
                char* buff = (char*)realloc(destiny, memo);
                if (buff == NULL){
                    return ERROR_MALLOC;
                }
                destiny = buff;
            }
            destiny[index] = (number & 1) ? '1' : '0';
            index = ultimateInc(index);
            number >>= 1;
        }

        destiny[index] = '\0';
        int left = 0;
        int right = ultimateDec(index);
        while(left < right) {
            char temp = destiny[right];
            destiny[right] = destiny[left];
            destiny[left] = temp;
            left = ultimateInc(left);
            right = ultimateDec(right);
        }

        return SUCCESS;
    }
};

int main() {
    logical_values_array a(5);
    logical_values_array b(3);

    cout << "a: " << a.get_value() << endl; //101
    cout << "b: " << b.get_value() << endl; //11

    cout << "a & b: " << (a & b).get_value() << endl;   //1
    cout << "a | b: " << (a | b).get_value() << endl;   //111
    cout << "a ^ b: " << (a ^ b).get_value() << endl;   //110
    cout << "~a: " << (~a).get_value() << endl;                     //11111111111111111111111111111010
    cout << "a -> b: " << a.implication(b).get_value() << endl;     //11111111111111111111111111111011
    cout << "a <-> b: " << a.coimplication(b).get_value() << endl;  //11111111111111111111111111111001
    cout << "a == b: " << a.equalant(b).get_value() << endl;        //11111111111111111111111111111001
    cout << "a ↓ b: " << a.pearce(b).get_value() << endl;           //11111111111111111111111111111000
    cout << "a | b: " << a.sheffer(b).get_value() << endl;          //11111111111111111111111111111110

    cout << "Bit 0 of a: " << a.get_bit(0) << endl;
    cout << "Bit 1 of a: " << a.get_bit(1) << endl;
    cout << "Bit 2 of a: " << a.get_bit(2) << endl;
    //cout << "Bit 3 of a: " << a.get_bit(3) << endl;
    cout << "Bit 0 of b: " << b.get_bit(0) << endl;
    cout << "Bit 1 of b: " << b.get_bit(1) << endl;

    char* binary_str = (char*)malloc(sizeof(char) << 1);
    if (binary_str == NULL) {
        logError(ERROR_MALLOC);
        return ERROR_MALLOC;
    }

    int code = a.to_binary_string(binary_str);
    if (code) {
        logError(code);
        return code;
    }
    cout << "Binary representation of a: " << binary_str << endl;
    free(binary_str);

    logical_values_array c(5);
    cout << "a == c: " << logical_values_array::equals(a, c) << endl;
    cout << "a == b: " << logical_values_array::equals(a, b) << endl;


    return 0;
}