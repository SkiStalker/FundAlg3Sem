#include "foo.hpp"

#include <gtest/gtest.h>

class FooTest : public ::testing::Test {
   protected:
	Foo foo{5};
};

TEST_F(FooTest, returning_zero_test) { 
	EXPECT_EQ(this->foo.return_zero(), 5); 
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}