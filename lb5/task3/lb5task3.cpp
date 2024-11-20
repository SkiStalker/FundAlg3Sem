#include <iostream>
#include <bitset>
#include <cstring>
#include <string>
#include <stdexcept>

using namespace std;

class LogicalValuesArray {
private:
	unsigned int value;

public:
	explicit LogicalValuesArray(unsigned int val = 0) : value(val) {}

	unsigned int getValue() const {
		return value;
	}

	void setValue(unsigned int val) {
		value = val;
	}

	LogicalValuesArray inversion() const {
		return LogicalValuesArray(~value);
	}

	LogicalValuesArray conjunction(const LogicalValuesArray& other) const {
		return LogicalValuesArray(value & other.value);
	}

	LogicalValuesArray disjunction(const LogicalValuesArray& other) const {
		return LogicalValuesArray(value | other.value);
	}

	LogicalValuesArray implication(const LogicalValuesArray& other) const {
		return LogicalValuesArray(~value | other.value);
	}

	LogicalValuesArray coimplication(const LogicalValuesArray& other) const {
		return LogicalValuesArray(~(value ^ other.value));
	}

	LogicalValuesArray xorOperation(const LogicalValuesArray& other) const {
		return LogicalValuesArray(value ^ other.value);
	}

	LogicalValuesArray equivalence(const LogicalValuesArray& other) const {
		return this->xorOperation(other).inversion();
	}

	LogicalValuesArray norOperation(const LogicalValuesArray& other) const {
		return LogicalValuesArray(~(value | other.value));
	}

	LogicalValuesArray nandOperation(const LogicalValuesArray& other) const {
		return LogicalValuesArray(~(value & other.value));
	}

	static bool equals(const LogicalValuesArray& a, const LogicalValuesArray& b) {
		return a.value == b.value;
	}

	bool getBit(unsigned int position, bool& bitValue) const {
		if (position >= sizeof(unsigned int) * 8) {
			bitValue = false;
			return false; 
		}
		bitValue = (value >> position) & 1;
		return true; 
	}


	bool toBinaryString(char* buffer, size_t size) const {
		if (size < sizeof(unsigned int) * 8 + 1) {
			return false;
		}
		bitset<sizeof(unsigned int) * 8> bits(value);
		string binaryStr = bits.to_string();
		strcpy(buffer, binaryStr.c_str());
		return true; 
	}

};

int main() {
	LogicalValuesArray a(5);
	LogicalValuesArray b(3);

	cout << "a = " << a.getValue() << "\n";
	cout << "b = " << b.getValue() << "\n";

	LogicalValuesArray result = a.conjunction(b);
	cout << "a AND b = " << result.getValue() << "\n";

	result = a.disjunction(b);
	cout << "a OR b = " << result.getValue() << "\n";

	result = a.implication(b);
	cout << "a -> b = " << result.getValue() << "\n";

	result = a.equivalence(b);
	cout << "a <-> b = " << result.getValue() << "\n";

	result = a.norOperation(b);
	cout << "a NOR b = " << result.getValue() << "\n";

	result = a.nandOperation(b);
	cout << "a NAND b = " << result.getValue() << "\n";

	char binaryBuffer[33];
	if (a.toBinaryString(binaryBuffer, sizeof(binaryBuffer))) {
		cout << "Binary representation of a: " << binaryBuffer << "\n";
	}
	else {
		cout << "Error: Buffer size too small\n";
	}


	bool bitValue;
	if (a.getBit(2, bitValue)) {
		cout << "Bit 2 of a: " << bitValue << "\n";
	}
	else {
		cout << "Error: Bit position out of range\n";
	}

	cout << "a and b are " << (LogicalValuesArray::equals(a, b) ? "equal" : "not equal") << "\n";

	return 0;
}