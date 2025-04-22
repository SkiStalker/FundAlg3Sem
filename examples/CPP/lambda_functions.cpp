#include <iostream>

class Functor {
	int a;

   public:
	Functor(int a) : a(a) {}

	void operator()(auto item) {
		std::cout << item << std::endl;
		std::cout << this->a << std::endl;
	}
};

int main() {
	int a = 12;

	auto func =
	    [&a](auto item) -> void {
		std::cout << item << std::endl;
        a++;
		std::cout << a << std::endl;
	};

	func("Hello world!!!");

	Functor{a}("Hello world!!!");

	return 0;
}