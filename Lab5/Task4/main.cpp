#include <iostream>
#include <cmath>

using namespace std;

enum ret_type_t
{
    SUCCESS,
    ERROR_MALLOC
};

void status_return(int code)
{
    switch (code)
    {
    case ERROR_MALLOC:
    {
        cout << "Failed to allocate memory" << endl;
    }
    }
}

class Complex
{
private:
    double real;
    double imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    Complex operator+(const Complex &other) const
    {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator-(const Complex &other) const
    {
        return Complex(real - other.real, imag - other.imag);
    }

    Complex operator*(const Complex &other) const
    {
        return Complex(real * other.real - imag * other.imag,
                       real * other.imag + imag * other.real);
    }

    Complex operator/(const Complex &other) const
    {
        if (other.imag == 0 && other.real == 0)
        {
            return Complex(0, 0);
        }
        double denominator = other.real * other.real + other.imag * other.imag;
        return Complex((real * other.real + imag * other.imag) / denominator,
                       (imag * other.real - real * other.imag) / denominator);
    }

    double modulus() const
    {
        return sqrt(real * real + imag * imag);
    }

    double argument() const
    {
        return atan2(imag, real);
    }

    void display() const
    {
        cout << real << " + " << imag << "i" << endl;
    }
};

int main()
{
    Complex z1(3, 4);
    Complex z2(1, -1);

    cout << "z1 = ";
    z1.display();
    cout << "z2 = ";
    z2.display();

    Complex sum = z1 + z2;
    cout << "Sum z1 + z2 = ";
    sum.display();

    Complex diff = z1 - z2;
    cout << "Subtract z1 - z2 = ";
    diff.display();

    Complex product = z1 * z2;
    cout << "Multiply z1 * z2 = ";
    product.display();

    Complex quotient = z1 / z2;
    cout << "Devision z1 / z2 = ";
    quotient.display();

    cout << "Modulus z1 = " << z1.modulus() << endl;
    cout << "Argument z1 = " << z1.argument() << " rad" << endl;

    return SUCCESS;
}