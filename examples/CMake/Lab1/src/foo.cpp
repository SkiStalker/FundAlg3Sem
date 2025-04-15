#include "foo.hpp"

Foo::Foo(int val) : _val(val) {}

Foo::~Foo() {}

int Foo::return_zero() const { return this->_val; }
