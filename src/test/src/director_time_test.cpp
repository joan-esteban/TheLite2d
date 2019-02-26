#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "TheLite2d/events/Event.h"
#include "TheLite2d/actions/director.h"
#include "TheLite2d/Timer.h"

using namespace testing;
using namespace thelite2d;
using namespace thelite2d::actions;

const double ABS_ERROR = 0.000001;

class DirectorTimeTest : public Test {
public:
	

	virtual void SetUp() {
	}
};

TEST_F(DirectorTimeTest, test1) {
	Timer timer;
	DirectorTime director{ 5 };
	director.update(timer.current());
	timer.next(std::chrono::milliseconds{ 1000 });
	director.update(timer.current());
	ASSERT_THAT(director.haveReachEnd(), Eq(false));

	timer.next(std::chrono::milliseconds{ 4000 });
	director.update(timer.current());
	ASSERT_THAT(director.haveReachEnd(), Eq(false));

}

TEST_F(DirectorTimeTest, test_upper_limit_is_not_end_but_100percent) {
	Timer timer;
	DirectorTime director{ 5 };
	director.update(timer.current());
	timer.next(std::chrono::milliseconds{ 5000 });
	director.update(timer.current());
	ASSERT_THAT(director.haveReachEnd(), Eq(false));

	ASSERT_NEAR(director.getPercentage().end, 1, 0.000000001f);
	ASSERT_NEAR(director.getPercentage().start, 0, 0.000000001f);
}

TEST_F(DirectorTimeTest, test_beyond_upper_limit_reach_end_and_more_than_100percent) {
	Timer timer;
	DirectorTime director{ 5 };
	director.update(timer.current());
	timer.next(std::chrono::milliseconds{ 6000 });
	director.update(timer.current());
	ASSERT_THAT(director.haveReachEnd(), Eq(true));

	ASSERT_NEAR(director.getPercentage().end, 1.2, 0.000000001f);
	ASSERT_NEAR(director.getPercentage().start, 0, 0.000000001f);
}
