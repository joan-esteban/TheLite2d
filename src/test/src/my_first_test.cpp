#include "gtest/gtest.h"
#include "TheLite2d/impl/Positionable.h"
#include "TheLite2d/SDLWrapper.h"
int Factorial(int n) { return n; }

TEST(FactorialTest, Negative) {
	// This test is named "Negative", and belongs to the "FactorialTest"
	// test case.
	EXPECT_EQ(1, Factorial(1));
	EXPECT_EQ(1, Factorial(1));
	EXPECT_GT(Factorial(1), 0);
}

TEST(FactorialTest, KK) {
	thelite2d::impl::Positionable pos;
	EXPECT_EQ(1.0, pos.getAlpha());
	thelite2d::SDLWrapper sdl;
	sdl.getDimensions();

}