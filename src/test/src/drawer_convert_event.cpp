#include "gtest/gtest.h"

#include "TheLite2d/Drawer.h"
#include "TheLite2d/events/Event.h"
using namespace testing;
using namespace thelite2d;

class EventDrawerTest : public Test {
public:
	Drawer  drawerIdentity;
	Drawer  drawerOffset;
	Drawer  drawerScale;
	Drawer  drawerScaleAndOffset;
	events::Event eventAt0_0{ events::EventType::MOUSE_BUTTON_DOWN,{ 0,0 },{} };
	events::Event eventAt100_200{ events::EventType::MOUSE_BUTTON_DOWN,{ 100,200 },{} };
	virtual void SetUp() {
		drawerIdentity.setPosition({ 0,0 });
		drawerIdentity.setScale({ 1,1 });

		drawerOffset.setPosition({ 100,50 });
		drawerOffset.setScale({ 1,1 });

		drawerScale.setPosition({ 0,0 });
		drawerScale.setScale({ 0.5,0.5});

		drawerScaleAndOffset.setPosition({ 100,200 });
		drawerScaleAndOffset.setScale({ 0.5,0.5 });
	}
};

TEST_F(EventDrawerTest, drawer_without_transformations_doesnt_transform_events) {
	Drawer  drawer;
	events::Event evMod{ eventAt0_0 };
	drawerIdentity.transformEvent(&evMod);
	ASSERT_NEAR(eventAt0_0.getWhere().x, evMod.getWhere().x,0.01);
	ASSERT_NEAR(eventAt0_0.getWhere().y, evMod.getWhere().y, 0.01);
}

TEST_F(EventDrawerTest, event_is_moved_to_relative_position_of_drawer) {
	events::Event evMod{ eventAt0_0 };
	drawerOffset.transformEvent(&evMod);
	ASSERT_NEAR(evMod.getWhere().x, -drawerOffset.getPosition().x, 0.01);
	ASSERT_NEAR(evMod.getWhere().y, -drawerOffset.getPosition().y, 0.01);
}

TEST_F(EventDrawerTest, event_is_scaled_to_relative_position_of_drawer) {
	events::Event evMod{ eventAt100_200 };
	drawerScale.transformEvent(&evMod);
	ASSERT_NEAR(evMod.getWhere().x, eventAt100_200.getWhere().x / drawerScale.getScale().x, 0.01);
	ASSERT_NEAR(evMod.getWhere().y, eventAt100_200.getWhere().y / drawerScale.getScale().y, 0.01);

}

TEST_F(EventDrawerTest, event_is_scaled_and_moved_to_relative_position_of_drawer) {
	events::Event evMod{ eventAt100_200 };
	drawerScaleAndOffset.transformEvent(&evMod);
	ASSERT_NEAR(evMod.getWhere().x, 0 , 0.01);
	ASSERT_NEAR(evMod.getWhere().y, 0, 0.01);

}