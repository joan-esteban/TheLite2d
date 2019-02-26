#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include "TheLite2d/Object.h"
#include "TheLite2d/actions/Execute.h"
#include "TheLite2d/impl/Insertable.h"
using namespace testing;
using namespace thelite2d::actions::Execute;
using thelite2d::Object;
using std::string;

class MyInsertable : public Object, public thelite2d::impl::Insertable
{

};

class ActionExecuteTest : public Test {

public:
	void SetUp() override {

	};

	Object object;
	MyInsertable insertable;

	class OnCallback {
	public:
		static bool callback(Object *notifier, Object *object) {
			callTimes++;
			return true;
		}
		static void clear() {
			callTimes = 0;
		}
		static int callTimes;
	};
};

int ActionExecuteTest::OnCallback::callTimes = 0;

TEST_F(ActionExecuteTest, callback_onStart_is_called_at_initialize) {
	OnCallback::clear();
	thelite2d::actions::Execute::ExecuteFunction f = OnCallback::callback;

	thelite2d::actions::Execute::Seq seq{ f, nullptr, nullptr };
	seq.initialize(object);

	EXPECT_THAT(OnCallback::callTimes, Eq(1));
	

}

TEST_F(ActionExecuteTest, callback_onStart_is_called_each_time_is_call_initialize) {
	OnCallback::clear();
	thelite2d::actions::Execute::ExecuteFunction f = OnCallback::callback;

	thelite2d::actions::Execute::Seq seq{ f, nullptr, nullptr };
	seq.initialize(object);
	seq.initialize(object);
	EXPECT_THAT(OnCallback::callTimes, Eq(2));
}

TEST_F(ActionExecuteTest, callback_onFinish_is_called_each_time_is_called_finish) {
	OnCallback::clear();
	thelite2d::actions::Execute::ExecuteFunction f = OnCallback::callback;

	thelite2d::actions::Execute::Seq seq{ nullptr, f, nullptr };
	seq.initialize(object);

	seq.finish(object);
	seq.finish(object);
	EXPECT_THAT(OnCallback::callTimes, Eq(2));
}

TEST_F(ActionExecuteTest, callback_onAbort_is_called_at_destroy_object_if_not_is_called_finish) {
	OnCallback::clear();
	thelite2d::actions::Execute::ExecuteFunction f = OnCallback::callback;

	{
		thelite2d::actions::Execute::Seq seq{ nullptr, nullptr, f };
		seq.initialize(object);
	}

	EXPECT_THAT(OnCallback::callTimes, Eq(1));
}

TEST_F(ActionExecuteTest, callback_onAbort_is_not_called_at_destroy_object_if_is_called_finish) {
	OnCallback::clear();
	thelite2d::actions::Execute::ExecuteFunction f = OnCallback::callback;

	{
		thelite2d::actions::Execute::Seq seq{ nullptr, nullptr, f };
		seq.initialize(object);
		seq.finish(object);
	}

	EXPECT_THAT(OnCallback::callTimes, Eq(0));
}
TEST_F(ActionExecuteTest, rtti_must_be_enable_at_compiling_time) {
	Object *obj = &insertable;
	thelite2d::IInsertable *p = dynamic_cast<thelite2d::IInsertable *>(obj);
	EXPECT_THAT(p, Ne(nullptr));
}

TEST_F(ActionExecuteTest, onStart_is_not_called_if_target_object_is_insertable_and_dont_have_parent) {
	OnCallback::clear();
	thelite2d::actions::Execute::ExecuteFunction f = OnCallback::callback;
	thelite2d::actions::Execute::Seq seq{ f, nullptr, nullptr };

	seq.initialize(insertable);

	EXPECT_THAT(OnCallback::callTimes, Eq(0));
}

