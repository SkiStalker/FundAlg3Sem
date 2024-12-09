#include <iostream>
#include <cmath>

using namespace std;

class Complex {
private:
    double real, imag;
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    Complex add(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex subtract(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    Complex multiply(const Complex& other) const {
        double r = real * other.real - imag * other.imag;
        double i = real * other.imag + imag * other.real;
        return Complex(r, i);
    }

    Complex divide(const Complex& other) const {
        double denominator = other.real * other.real + other.imag * other.imag;
        if (denominator == 0) throw runtime_error("Division by zero.");
        double r = (real * other.real + imag * other.imag) / denominator;
        double i = (imag * other.real - real * other.imag) / denominator;
        return Complex(r, i);
    }

    double modulus() const {
        return sqrt(real * real + imag * imag);
    }

    double argument() const {
        return atan2(imag, real);
    }
    
    void print() const {
        cout << real;
        if (imag >= 0) cout << " + " << imag << "i";
        else cout << " - " << -imag << "i";
    }
};

int main() {
    Complex c1(3, 4), c2(1, -2);
    cout << "Первое комплексное число: ";
    c1.print();
    cout << "\nВторое комплексное число: ";
    c2.print();
    Complex sum = c1.add(c2);
    cout << "\n\nСумма: ";
    sum.print();
    Complex diff = c1.subtract(c2);
    cout << "\nРазность: ";
    diff.print();
    Complex prod = c1.multiply(c2);
    cout << "\nПроизведение: ";
    prod.print();
    try {
        Complex quot = c1.divide(c2);
        cout << "\nЧастное: ";
        quot.print();
    } catch (const runtime_error& e) {
        cout << "\nОшибка: " << e.what();
    }
    cout << "\n\nМодуль первого числа: " << c1.modulus();
    cout << "\nАргумент первого числа (в радианах): " << c1.argument();
    return 0;
}
