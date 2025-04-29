#include <iostream>
#include <memory>

struct my_struct {
	int a;
	int b;
	double c;

	~my_struct() { std::cout << "Remove" << std::endl; } // some removes printed because move operation occurred
};



struct my_struct_1 {
	int a;
	int b;
	double c;

	~my_struct_1() { std::cout << "Remove 1" << std::endl; } // some removes printed because move operation occurred
};

std::shared_ptr<my_struct> my_str_ptr_shared_global; // only for example

int main() {
    std::cout << "int make" << std::endl;
	std::unique_ptr<int> int_ptr = std::make_unique<int>(12);

    std::cout << "int new" << std::endl;

	std::unique_ptr<int> int_ptr1 = std::unique_ptr<int>(new int{12});


    
	{
        std::cout << "my_struct make" << std::endl;
		std::unique_ptr<my_struct> my_str_ptr = std::make_unique<my_struct>(my_struct{1, 2, 1.1});

        std::cout << "shared make" << std::endl;
		std::shared_ptr<my_struct> my_str_ptr_shared = std::make_shared<my_struct>(my_struct{1, 2, 1.1});

        std::cout << "shared copy" << std::endl;
        my_str_ptr_shared_global = my_str_ptr_shared;
	}

    std::cout << "unique array make" << std::endl;
    
    std::unique_ptr<my_struct[]> my_str_ptr_1 = std::make_unique<my_struct[]>(12);
    std::cout << "unique array constr" << std::endl;
    std::unique_ptr<my_struct[]> my_str_ptr_2(new my_struct[12]);



	return 0;
}