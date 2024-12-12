#include <iostream>
#include <cstdlib>
using namespace std;

unsigned increment(unsigned x) {
    unsigned mask;
    for (mask = 1; (x & mask); mask <<= 1)
        x &= ~mask;
    x |= mask;
    return x;
}

int negative(int x) {
    return increment(~x);
}

int decrement(int x) {
    return negative(increment(increment(~x)));
}

class logical_values_array {
private:
    unsigned value;
public:
    unsigned access() {
        return value;
    }
    void set(int val) {
        value = val;
    }
    logical_values_array(unsigned val = 0) {
        value = val;
    }
    logical_values_array inv() const {
        return logical_values_array(~value);
    }
    logical_values_array cnj(const logical_values_array& lva) const {
        return logical_values_array(value & lva.value);
    }
    logical_values_array disj(const logical_values_array& lva) const {
        return logical_values_array(value | lva.value);
    }
    logical_values_array shaffer(const logical_values_array& lva) const {
        return ((*this).cnj(lva)).inv();
    }
    logical_values_array pearce(const logical_values_array& lva) const {
        return ((*this).disj(lva)).inv();
    }
    logical_values_array impl(const logical_values_array& lva) const {
        return lva.disj((*this).inv());
    }
    logical_values_array coimpl(const logical_values_array& lva) const {
        return ((*this).impl(lva)).inv();
    }
    logical_values_array xoor(const logical_values_array& lva) const {
        return ((*this).inv().cnj(lva)).disj((*this).cnj(lva.inv()));
    }
    logical_values_array equal(const logical_values_array& lva) const {
        return ((*this).xoor(lva)).inv();
    }
    int get_bit(int pos) const {
        if (pos < 0)
            throw("Incorrect position");
        return (value >> pos) & 1;
    }
    void to_binary(char* s) {
        int i = 0, val = value;
        while (val) {
            s[i] = (val & 1) ? '1' : '0';
            i = increment(i);
            val >>= 1;
        }
        s[i] = '\0';
        int left = 0, right = decrement(i);
        while (left < right) {
            char tmp = s[left];
            s[left] = s[right];
            s[right] = tmp;
            left = increment(left);
            right = decrement(right);
        }
    }
    static bool equals(const logical_values_array& lv1, const logical_values_array& lv2) {
        return lv1.value == lv2.value;
    }
};

int main()
{
    logical_values_array lva(13), lvb(9), lvc(13);
    cout << lva.access() << ' ' << lvb.access() << endl;
    cout << lva.inv().access() << endl;
    cout << lva.cnj(lvb).access() << endl;
    cout << lva.disj(lvb).access() << endl;
    cout << lva.pearce(lvb).access() << endl;
    cout << lva.shaffer(lvb).access() << endl;
    cout << lva.impl(lvb).access() << endl;
    cout << lva.xoor(lvb).access() << endl;
    cout << lva.coimpl(lvb).access() << endl;
    cout << lva.equal(lvb).access() << endl;
    cout << logical_values_array::equals(lva, lvb) << ' ' << logical_values_array::equals(lva, lvc) << endl;

    try {
        cout << lva.get_bit(0) << ' ' << lva.get_bit(1) << ' ' << lva.get_bit(2) << endl;
    }
    catch (char* e) {
        cout << e << endl;
        return 1;
    }

    logical_values_array a(8);
    char binary_str[33];
    a.to_binary(binary_str);
    cout << "a = 8; binary representation of a: " << binary_str << endl;

    return 0;
}
