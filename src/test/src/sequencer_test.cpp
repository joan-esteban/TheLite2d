#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "TheLite2d/Object.h"
#include "TheLite2d/actions/Sequencer.h"
#include "TheLite2d/events/Event.h"
#include "TheLite2d/actions/Execute.h"

using namespace testing;
using namespace thelite2d;
using namespace thelite2d::actions;
using thelite2d::Object;

const double ABS_ERROR = 0.000001;

class MockIActionDirectable : public IActionDirectable {
public:
	MOCK_METHOD1(initialize, bool(Object &objectToControl));
	MOCK_METHOD2(step, bool(SegmentPercent percent, Object &objectToControl));
	MOCK_METHOD1(finish, void(Object &objectToControl));
};
class SequencerTest : public Test {
public:
	Sequencer<DirectorForced> sequencer{ DirectorForced{} };
	std::shared_ptr<MockIActionDirectable> action = std::make_shared<MockIActionDirectable>();
	std::shared_ptr<MockIActionDirectable> actionTwo = std::make_shared<MockIActionDirectable>();
	std::shared_ptr<MockIActionDirectable> actionThree = std::make_shared<MockIActionDirectable>();
	std::shared_ptr<Object> object = std::make_shared<Object>();
	Present present;

	virtual void SetUp() {
	}
};

TEST_F(SequencerTest, sequencer_finish_at_100_percent) {
	//ASSERT_TRUE(rect1.isInside({ 1,1 }));
	sequencer.add(0, 1, object, action);
	sequencer.getDirector().set( { 0,1 } , false);
	ASSERT_THAT(sequencer.step(present, *object.get()), Eq(false));
}

TEST_F(SequencerTest, sequencer_call_to_initialize_action) {
	sequencer.add(0, 1, object, action);
	sequencer.getDirector().set({ 0,0.5 }, false);

	EXPECT_CALL(*action, initialize(_))
		.Times(AtLeast(1));

	sequencer.step(present, *object);
}

TEST_F(SequencerTest, sequencer_call_to_step_action) {
	sequencer.add(0, 1, object, action);
	sequencer.getDirector().set({ 0,0.5 }, false);

	EXPECT_CALL(*action, step(_,_))
		.Times(AtLeast(1));

	sequencer.step(present, *object);
}

TEST_F(SequencerTest, sequencer_clear_actions) {
	sequencer.add(0, 1, object, action);
	sequencer.clear();
	sequencer.getDirector().set({ 0,0.5 }, false);

	EXPECT_CALL(*action, step(_, _))
		.Times(0);

	sequencer.step(present, *object);
}

TEST_F(SequencerTest, sequencer_call_to_step_relative_percent) {
	sequencer.add(0, 0.5, object, action);
	sequencer.add(0.5, 1, object, actionTwo);
	sequencer.getDirector().set({ 0,0.25 }, false);

	EXPECT_CALL(*action, step(SegmentPercent{ 0,0.50 }, _))
		.Times(AtLeast(1));

	sequencer.step(present, *object);
}

TEST_F(SequencerTest, sequencer_a_action_is_always_executed_even_there_are_a_big_time_jump) {
	sequencer.add(0, 0.4, object, actionTwo);
	sequencer.add(0.4, 0.5, object, action);
	sequencer.add(0.5, 1, object, actionTwo);
	
	sequencer.getDirector().set({ 0,0.6 } , false);

	EXPECT_CALL(*action, initialize(_))
		.Times(AtLeast(1));

	sequencer.step(present, *object);
}

TEST_F(SequencerTest, sequencer_a_milestone_action_at_end_of_sequence_is_called) {
	sequencer.add(0, 1, object, actionTwo);
	sequencer.add(1, 1, object, action);
	// 0.99999
	sequencer.getDirector().set({ 0.999991,0.999992 }, false);

	EXPECT_CALL(*action, initialize(_))
		.Times(AtLeast(1));

	sequencer.step(present, *object);
}

TEST_F(SequencerTest, sequencer_dont_must_finish_until_reach_last_item_action) {
	bool executeFuncCalled = false;
	actions::Execute::ExecuteFunction  func = [&](Object *notifier, Object *object) { executeFuncCalled = true;  return true; };

	auto seq = std::make_shared<actions::Sequencer<> >(actions::DirectorTime{ 5.0 });
	seq->add(0.0, 1, actions::Execute::seqInstance(actions::Execute::FunctionNop));
	seq->add(1, 1, actions::Execute::seqInstance(func));

	Timer timer;
	Object object;
	seq->step(timer.current(), object);

	timer.next(std::chrono::milliseconds{ (5 * 1000 )-5 });
	// True means that seq is not finish
	EXPECT_THAT(seq->step(timer.current(), object), Eq(true));
	EXPECT_THAT(executeFuncCalled, Eq(false));

	timer.next(std::chrono::milliseconds{ 10 });

	// Seq have finished
	EXPECT_THAT(seq->step(timer.current(), object), Eq(false));
	// In this step have been called last execute action
	EXPECT_THAT(executeFuncCalled, Eq(true));
}



TEST_F(SequencerTest, sequencer_calculate_time_scale_to_contained_items) {
	sequencer.add(-10, 40, object, actionTwo);
	sequencer.add(40, 50, object, action);
	sequencer.add(50, 100, object, action);

	auto duration = sequencer.getTimeScale();

	ASSERT_NEAR(duration.start,-10, ABS_ERROR);
	ASSERT_NEAR(duration.end, 100, ABS_ERROR);
}

TEST_F(SequencerTest, sequencer_empty_have_default_time_scale) {
	auto duration = sequencer.getTimeScale();

	ASSERT_NEAR(duration.start, 0, ABS_ERROR);
	ASSERT_NEAR(duration.end,   1, ABS_ERROR);
}
MATCHER_P(EqSegment, value, "") { return (abs(value.start-arg.start)<ABS_ERROR) && (abs(value.end - arg.end)<ABS_ERROR); }


TEST_F(SequencerTest, sequencer_calls_to_relative_time_scale_to_contained_items) {
	sequencer.add(0, 50, object, action);
	sequencer.add(50, 100, object, actionTwo);

	sequencer.getDirector().set({ 0,0.49}, false);
	auto duration = sequencer.getTimeScale();

	EXPECT_CALL(*action, step(EqSegment(SegmentPercent{ 0,0.98 }) , _))
		.Times(1);

	sequencer.step(present, *object);
}
TEST_F(SequencerTest, sequencer_adjustPercentToItems) {
	auto segment = sequencer.adjustPercentToItems({ 0,0.5 }, { -50, 50 });
	EXPECT_THAT(segment, EqSegment(SegmentPercent{ -50,0 }) );

}

TEST_F(SequencerTest, sequencer_calculateChildPercent) {
	auto segment = sequencer.calculateChildPercent({ -50,50 }, { -50, 0 });
	EXPECT_THAT(segment, EqSegment(SegmentPercent{ 0,0.50 }));

}

TEST_F(SequencerTest, sequencer_calls_to_relative_time_scale_to_contained_items_even_on_negative_scale) {
	sequencer.add(-50, 50, object, action);
	sequencer.add(50, 150, object, actionTwo);

	sequencer.getDirector().set({ 0,0.49 }, false);
	auto duration = sequencer.getTimeScale();

	EXPECT_CALL(*action, step(EqSegment(SegmentPercent{ 0,0.98 }), _))
		.Times(1);

	sequencer.step(present, *object);
}

TEST_F(SequencerTest, sequencer_calls_to_milestone_at_end) {
	sequencer.add(-50, 50, object, actionTwo);
	sequencer.add(50, 150, object, actionTwo);
	sequencer.add(200, 200, object, action);

	sequencer.getDirector().set({ 0,1 }, true);
	

	EXPECT_CALL(*action, initialize(_))
		.Times(1);

	sequencer.step(present, *object);
}

/*
TEST_F(SequencerTest, sequencer_call_to_action2) {
	//ASSERT_TRUE(rect1.isInside({ 1,1 }));
	sequencer.add(0, 0.49, object, action);
	sequencer.add(0.5, 1, object, actionTwo);
	sequencer.getDirector().set(0.1, false);
	EXPECT_CALL(*action.get(), step(_, _))
		.Times(1);

	sequencer.getDirector().set(0.6, false);
	EXPECT_CALL(*action.get(), finish(_))
		.Times(1);
	EXPECT_CALL(*actionTwo.get(), step(_, _))
		.Times(1);
}

TEST_F(SequencerTest, sequencer_call_to_action_relative) {
	//ASSERT_TRUE(rect1.isInside({ 1,1 }));
	sequencer.add(0, 0.49, object, action);
	sequencer.add(RelativeToEndPrevious{ -0.10 }, 1, object, actionTwo);
	sequencer.getDirector().set(0.1, false);
	EXPECT_CALL(*action.get(), step(_, _))
		.Times(1);

	sequencer.getDirector().set(0.6, false);
	EXPECT_CALL(*action.get(), finish(_))
		.Times(1);
	EXPECT_CALL(*actionTwo.get(), step(_, _))
		.Times(1);
}
*/
