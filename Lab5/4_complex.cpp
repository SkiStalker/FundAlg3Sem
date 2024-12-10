#include <iostream>

using namespace std;

class complex {
private:
    double real;
    double imag;

public:
    complex(double r = 0, double i = 0) : real(r), imag(i) {}

    complex operator+(const complex& other) {
        double r = real + other.real;
        double i = imag + other.imag;
        return complex(r, i); 
    }

    complex operator-(const complex& other) {
        double r = real - other.real;
        double i = imag - other.imag;
        return complex(r, i); 
    }

    complex operator*(const complex& other) {
        double r = real * other.real - imag * other.imag;
		double i = real * other.imag + imag * other.real;
		return complex(r, i);
    }

    complex operator/(const complex& other) const {
        double denominator = other.real * other.real + other.imag * other.imag;
        if (denominator == 0) {
            throw runtime_error("Деление на ноль");
        }
        double r = (real * other.real + imag * other.imag) / denominator;
        double i = (imag * other.real - real * other.imag) / denominator;
        return complex(r, i);
    }

    double modulus() const {
		return sqrt(real * real + imag * imag);
	}

	double argument() const {
		return atan2(imag, real);
	}

    void print() const {
        cout << real;
        if (imag >= 0) {
            cout << " + " << imag << "i";
        } else {
            cout << " - " << -imag << "i";
        }
        cout << endl;
    }
};

int main() {
    complex z1(3, 2);  // 3 + 2i
    complex z2(1, 4);  // 1 + 4i

    complex sum = z1 + z2;
    complex diff = z1 - z2;
    complex prod = z1 * z2;
    complex quot = z1 / z2;

    cout << "Сложение: ";
    sum.print();
    cout << "Вычитание: ";
    diff.print();
    cout << "Умножение: ";
    prod.print();
    cout << "Деление: ";
    quot.print();

    cout << "Модуль z1: " << z1.modulus() << endl;
    cout << "Аргумент z1: " << z1.argument() << " радиан" << endl;

    return 0;
}
