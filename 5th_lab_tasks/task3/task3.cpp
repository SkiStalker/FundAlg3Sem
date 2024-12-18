#include <iostream>
#include <stdexcept>
#include <string.h>

class logical_values_array final
{
private:
    unsigned int _value;

public:
    logical_values_array(unsigned int value = 0) { _value = value; }
    unsigned int value_accessor() const { return _value; }
    logical_values_array inversion() const { return logical_values_array(~_value); }
    logical_values_array conjunction(const logical_values_array &lva) const { return logical_values_array(lva.value_accessor() & _value); }
    logical_values_array disjunction(logical_values_array const &lva) const
    {
        logical_values_array tmp = lva.inversion();
        logical_values_array tmp_2 = this->inversion();
        return tmp_2.conjunction(tmp).inversion();
    }

    logical_values_array implication(logical_values_array const &lva) const { return this->inversion().disjunction(lva); }

    logical_values_array coimplication(logical_values_array const &lva) const { return this->conjunction(lva.inversion()); }

    logical_values_array add_mod2(logical_values_array const &lva) const
    {
        logical_values_array temp_1, temp_2;
        temp_1 = this->inversion().conjunction(lva);
        temp_2 = this->conjunction(lva.inversion());
        return temp_1.disjunction(temp_2);
    }

    logical_values_array equal(logical_values_array const &lva) const
    {
        return this->add_mod2(lva).inversion();
    }

    logical_values_array Pirs(logical_values_array const &lva) const
    {
        return this->disjunction(lva).inversion();
    }

    logical_values_array Sheffer(logical_values_array const &lva) const
    {
        return this->conjunction(lva).inversion();
    }

    static bool equals(logical_values_array const &lva_1, logical_values_array const &lva_2)
    {
        return lva_1.value_accessor() == lva_2.value_accessor();
    }

    bool get_bit(size_t position) const
    {
        if (position >= sizeof(_value) * 8)
            throw "Incorrect position";

        return (1ul << position) & _value;
    }

    void get_str_value(char *str_value) const
    {
        if (!str_value)
            throw "Null string";

        short count_bits = sizeof(_value) * 8;

        for (int i = 0; i < count_bits; ++i)
        {
            str_value[count_bits - 1 - i] = ((1 << i) & _value) ? '1' : '0';
        }
        str_value[count_bits] = '\0';
    }
};

int main()
{
    char arr[128];
    logical_values_array a(12032);
    logical_values_array b(112321);
    a.get_str_value(arr);
    std::cout << "a: " << arr << std::endl;

    b.get_str_value(arr);
    std::cout << "bF: " << arr << std::endl;

    logical_values_array c = a.inversion();
    c.get_str_value(arr);
    std::cout << "inversion a: " << arr << std::endl;

    c = a.conjunction(b);
    c.get_str_value(arr);
    std::cout << "conjunction: " << arr << std::endl;

    c = a.disjunction(b);
    c.get_str_value(arr);
    std::cout << "disjunction: " << arr << std::endl;

    c = a.implication(b);
    c.get_str_value(arr);
    std::cout << "implication: " << arr << std::endl;

    c = a.coimplication(b);
    c.get_str_value(arr);
    std::cout << "coimplication: " << arr << std::endl;

    c = a.add_mod2(b);
    c.get_str_value(arr);
    std::cout << "add_mod2: " << arr << std::endl;

    c = a.equal(b);
    c.get_str_value(arr);
    std::cout << "equal: " << arr << std::endl;

    c = a.Pirs(b);
    c.get_str_value(arr);
    std::cout << "pirs: " << arr << std::endl;

    c = a.Sheffer(b);
    c.get_str_value(arr);
    std::cout << "sheffer: " << arr << std::endl;

    try
    {
        bool bit = a.get_bit(1);
        std::cout << "a.get_bit(1) = " << bit << std::endl;
        bit = a.get_bit(10);
        std::cout << "a.get_bit(10) = " << bit << std::endl;
    }
    catch (const char *error_message)
    {
        std::cout << error_message << std::endl;
    }

    if (logical_values_array::equals(a, b))
    {
        std::cout << "a == b: True" << std::endl;
    }
    else
    {
        std::cout << "a == b: False" << std::endl;
    }
        c = a;
    if (logical_values_array::equals(a, c))
    {
        std::cout << "a == c: True" << std::endl;
    }
    else
    {
        std::cout << "a == c: False" << std::endl;
    }
}