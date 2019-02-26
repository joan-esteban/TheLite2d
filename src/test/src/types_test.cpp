#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "TheLite2d/types.h"
using namespace testing;
using namespace thelite2d;


TEST(DeltaPresent_isInside) {
	SegmentPercent deltaPercentFull = { 0,1 };
	ASSERT_THAT(deltaPercentFull.isInside(0.5), Eq(true));
	ASSERT_THAT(deltaPercentFull.isInside(1), Eq(true));
	ASSERT_THAT(deltaPercentFull.isInside(0), Eq(true));
	ASSERT_THAT(deltaPercentFull.isInside(-0.5), Eq(false));
	ASSERT_THAT(deltaPercentFull.isInside(1.5), Eq(false));
}

