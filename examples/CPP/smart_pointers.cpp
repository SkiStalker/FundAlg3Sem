#include <iostream>
#include <memory>

struct my_struct {
	int a;
	int b;
	double c;

	~my_struct() { std::cout << "Remove" << std::endl; } // some removes printed because move operation occurred
};


std::shared_ptr<my_struct> my_str_ptr_shared_global; // only for example

int main() {
	std::unique_ptr<int> int_ptr = std::make_unique<int>(12);

	std::unique_ptr<int> int_ptr1 = std::unique_ptr<int>(new int{12});

	{
		std::unique_ptr<my_struct> my_str_ptr = std::make_unique<my_struct>(my_struct{1, 2, 1.1});


		std::shared_ptr<my_struct> my_str_ptr_shared = std::make_shared<my_struct>(my_struct{1, 2, 1.1});

        my_str_ptr_shared_global = my_str_ptr_shared;
	}


	return 0;
}