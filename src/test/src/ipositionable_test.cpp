#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "TheLite2d/IPositionable.h"

using namespace testing;
using namespace thelite2d;

class TestTransform : public Test {
public:
	RectangleWidgetParams paramsIdentity;
	RectangleWidgetParams paramsScale;
	Point2d pointAt0_0 = { 0,0 };
	Point2d pointAt100_100 = { 100,100 };
	virtual void SetUp() {
		paramsIdentity.position = { 0,0 };
		paramsIdentity.scale = { 1,1 };
		paramsIdentity.calculatedPivotPosition = { 0,0 };

		paramsScale.position = { 0,0 };
		paramsScale.scale = { 0.5,0.5 };
		paramsScale.calculatedPivotPosition = { 0,0 };

	}
};

TEST_F(TestTransform, transform_identity_same_point) {
	Point2d transformed = paramsIdentity.transform(pointAt0_0);
	ASSERT_NEAR(pointAt0_0.x, transformed.x, 0.01);
	ASSERT_NEAR(pointAt0_0.y, transformed.y, 0.01);
}

TEST_F(TestTransform, inverseTransform_identity_same_point) {
	Point2d transformed = paramsIdentity.inverseTransform(pointAt0_0);
	ASSERT_NEAR(pointAt0_0.x, transformed.x, 0.01);
	ASSERT_NEAR(pointAt0_0.y, transformed.y, 0.01);
}

TEST_F(TestTransform, transform_with_scale) {
	Point2d transformed = paramsIdentity.transform(pointAt100_100);
	ASSERT_NEAR(pointAt100_100.x * paramsIdentity.scale.x, transformed.x, 0.01);
	ASSERT_NEAR(pointAt100_100.y * paramsIdentity.scale.y, transformed.y, 0.01);
}

TEST_F(TestTransform, inverseTransform_with_scale) {
	Point2d transformed = paramsIdentity.transform(pointAt100_100);
	ASSERT_NEAR(pointAt100_100.x / paramsIdentity.scale.x, transformed.x, 0.01);
	ASSERT_NEAR(pointAt100_100.y / paramsIdentity.scale.y, transformed.y, 0.01);
}
