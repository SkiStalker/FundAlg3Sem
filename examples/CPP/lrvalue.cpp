#include <iostream>
#include <thread>

int x = 0;
int val() { return 0; }
int& ref() { return x; }

void test(int&) { std::cout << "lvalue\n"; }

void test(int&&) { std::cout << "rvalue\n"; }

int main() {
    
	test(0);             // rvalue
	test(x);             // lvalue
	test(val());         // rvalue
	test(ref());         // lvalue

	test(std::move(x));  // rvalue

	return 0;
}