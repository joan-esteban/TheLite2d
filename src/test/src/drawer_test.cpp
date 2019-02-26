#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "TheLite2d/Drawer.h"
using namespace testing;
using namespace thelite2d;

class MockInsertable : public IInsertable {
public:
	MOCK_METHOD1(setParent, void(IContainer*));
	MOCK_CONST_METHOD0(getParent, IContainer*());
};

class MyInsertable : public IInsertable {
public:
	virtual ~MyInsertable() = default;
	virtual void setParent(IContainer *newParent) override { parent = newParent; };
	virtual IContainer *getParent() const override { return parent; };
	IContainer *parent = nullptr;
};

class DrawerTest : public Test {
public:
	Drawer drawer;
	virtual void SetUp() override {

	}
};

TEST_F(DrawerTest, addItems_set_parent_to_item) {
	auto item = std::make_shared<MockInsertable>();
	EXPECT_CALL(*item.get(), setParent(_))
		.Times(AtLeast(1));
	drawer.add(item);
}

TEST_F(DrawerTest, getItems_return_inserted_items) {
	auto item = std::make_shared<MockInsertable>();
	drawer.add(item);

	auto  items = drawer.getItems<IInsertable>();
	EXPECT_THAT(items.size(), Eq(1));
}

TEST_F(DrawerTest, getItems_function_just_returns_items_that_satisfy_interface) {
	auto item = std::make_shared<MockInsertable>();
	drawer.add(item);

	auto  items = drawer.getItems<IDrawable>();
	EXPECT_THAT(items.size(), Eq(0));
}

TEST_F(DrawerTest, remove_shared_object_delete_item) {
	auto item = std::make_shared<MyInsertable>();
	drawer.add(item);
	drawer.remove(item);

	auto  items = drawer.getItems<IInsertable>();
	EXPECT_THAT(items.size(), Eq(0));
}

TEST_F(DrawerTest, DISABLED_remove_raw_pointer_delete_item) {
	auto item = std::make_shared<MyInsertable>();
	drawer.add(item);
	drawer.remove( dynamic_cast<IInsertable*>(item.get()) );

	auto  items = drawer.getItems<IInsertable>();
	EXPECT_THAT(items.size(), Eq(0));
}

TEST_F(DrawerTest, DISABLED_remove_no_inserted_pointer_delete_item) {
	auto item = std::make_shared<MyInsertable>();
	auto item2 = std::make_shared<MyInsertable>();
	drawer.add(item);
	drawer.remove(dynamic_cast<IInsertable*>(item2.get()));

	auto  items = drawer.getItems<IInsertable>();
	EXPECT_THAT(items.size(), Eq(1));
}

TEST_F(DrawerTest, DISABLED_remove_twice_pointer_delete_item) {
	auto item = std::make_shared<MyInsertable>();
	
	drawer.add(item);
	drawer.remove(dynamic_cast<IInsertable*>(item.get()));
	//drawer.remove(dynamic_cast<IInsertable*>(item.get()));

	auto  items = drawer.getItems<IInsertable>();
	EXPECT_EQ(items.size(), 0);
}