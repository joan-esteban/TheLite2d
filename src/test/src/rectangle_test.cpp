#include "gtest/gtest.h"

#include "TheLite2d/Drawer.h"
#include "TheLite2d/events/Event.h"
using namespace testing;
using namespace thelite2d;

class RectangleTest : public Test {
public:
	thelite2d::Rectangle rect1{ {0,0}, {100,100} };
	virtual void SetUp() {
//		drawerIdentity.setPosition({ 0,0 });
	}
};

TEST_F(RectangleTest, inside) {
	ASSERT_TRUE(rect1.isInside({ 1,1 }));
}

