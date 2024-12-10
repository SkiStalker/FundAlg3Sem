#include <iostream>
#include <bitset>
#include <cstring>

using namespace std;

class logical_values_array {
private:
    unsigned int value;

public:
    explicit logical_values_array(unsigned int initValue = 0) : value(initValue) {}

    unsigned int getValue() const {
        return value;
    }

    void setValue(unsigned int newValue) {
        value = newValue;
    }

    logical_values_array invert() const {
        return logical_values_array(~value);
    }

    logical_values_array conjunction(const logical_values_array& other) const {
        return logical_values_array(value & other.value);
    }

    logical_values_array disjunction(const logical_values_array& other) const {
        return logical_values_array(value | other.value);
    }

    logical_values_array implication(const logical_values_array& other) const {
        return logical_values_array(~value | other.value);
    }

    logical_values_array coimplication(const logical_values_array& other) const {
        return logical_values_array(value & ~other.value);
    }

    logical_values_array xorOperation(const logical_values_array& other) const {
        return logical_values_array(value ^ other.value);
    }

    logical_values_array equivalence(const logical_values_array& other) const {
        return xorOperation(other).invert();
    }

    logical_values_array norOperation(const logical_values_array& other) const {
        return disjunction(other).invert();
    }

    logical_values_array nandOperation(const logical_values_array& other) const {
        return conjunction(other).invert();
    }

    static bool equals(const logical_values_array& first, const logical_values_array& second)  {
        return first.value == second.value;
    }

    bool getBit (unsigned int position) const {
        if (position >= sizeof(unsigned int) * 8) {
            throw out_of_range("Position is out of range");
        }
        return (value >> position) & 1;
    }

    void to_binary_string(char* buffer, size_t bufferSize) const {
        if (bufferSize < sizeof(unsigned int) * 8 + 1) {
            throw invalid_argument("Buffer size is too small");
        }
        bitset<sizeof(unsigned int) * 8> bits(value);
        strcpy(buffer, bits.to_string().c_str());
    }
};

int main() {
    logical_values_array a(0b1100);
    logical_values_array b(0b1010);

    cout << "A: " << a.getValue() << "\n";
    cout << "B: " << b.getValue() << "\n";

    // Операции
    cout << "A AND B: " << a.conjunction(b).getValue() << "\n";
    cout << "A OR B: " << a.disjunction(b).getValue() << "\n";
    cout << "A XOR B: " << a.xorOperation(b).getValue() << "\n";
    cout << "A -> B: " << a.implication(b).getValue() << "\n";
    cout << "A NOR B: " << a.norOperation(b).getValue() << "\n";
    cout << "A NAND B: " << a.nandOperation(b).getValue() << "\n";

    cout << "A == B: " << a.equivalence(b).getValue() << "\n";

    cout << "Бит A[2]: " << a.getBit(2) << "\n";

    char binaryString[33];
    a.to_binary_string(binaryString, sizeof(binaryString));
    cout << "A в двоичном виде: " << binaryString << "\n";

    return 0;
}