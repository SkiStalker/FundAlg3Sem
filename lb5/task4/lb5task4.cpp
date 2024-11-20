#include <iostream>
#include <cmath>

using namespace std;

class ComplexNumber {
private:
	double real;
	double imag;

public:
	ComplexNumber(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

	ComplexNumber add(const ComplexNumber& other) const {
		return ComplexNumber(real + other.real, imag + other.imag);
	}

	ComplexNumber subtract(const ComplexNumber& other) const {
		return ComplexNumber(real + other.real, imag + other.imag);
	}

	ComplexNumber multiply(const ComplexNumber& other) const {
		double r = real * other.real - imag * other.imag;
		double i = real * other.imag + imag * other.real;
		return ComplexNumber(r, i);
	}

	ComplexNumber divide(const ComplexNumber& other) const {
		double denominator = other.real * other.real + other.imag * other.imag;
		if (denominator == 0) {
			throw runtime_error("Division by zero");
		}
		double r = (real * other.real + imag * other.imag) / denominator;
		double i = (imag * other.real - real * other.imag) / denominator;
		return ComplexNumber(r, i);
	}

	double modulus() const {
		return sqrt(real * real + imag * imag);
	}

	double argument() const {
		return atan2(imag, real);
	}

	void display() const {
		cout << real << (imag >= 0 ? " + " : " - ") << std::abs(imag) << "i";
	}
};


int main() {
	ComplexNumber c1(3, 4);
	ComplexNumber c2(1, -2);

	cout << "Complex number 1: ";
	c1.display();
	cout << "\nComplex number 2: ";
	c2.display();

	ComplexNumber sum = c1.add(c2);
	cout << "\n\nSum: ";
	sum.display();

	ComplexNumber difference = c1.subtract(c2);
	cout << "\nDifference: ";
	difference.display();

	ComplexNumber product = c1.multiply(c2);
	cout << "\nProduct: ";
	product.display();

	try {
		ComplexNumber quotient = c1.divide(c2);
		cout << "\nQuotient: ";
		quotient.display();
	}
	catch (const exception& e) {
		cout << "\nDivision error: " << e.what();
	}

	cout << "\n\nModulus of c1: " << c1.modulus();
	cout << "\nArgument of c1: " << c1.argument() << " radians\n";

	return 0;
}