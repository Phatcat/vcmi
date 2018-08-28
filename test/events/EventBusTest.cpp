/*
 * events\EventBusTest.cpp, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#include "StdInc.h"

#include <vcmi/events/Event.h>

#include "../../lib/events/EventBus.h"

namespace test
{
using namespace ::testing;
using namespace ::events;

class EventExample : public Event
{
public:
	using PreHandler = SubscriptionRegistry<EventExample>::PreHandler;
	using PostHandler = SubscriptionRegistry<EventExample>::PostHandler;
	using BusTag = SubscriptionRegistry<EventExample>::BusTag;

public:
	MOCK_METHOD1(executeStub, void(const EventBus *));

	virtual void execute(const EventBus * bus)
	{
		executeStub(bus);
	}

	static SubscriptionRegistry<EventExample> * getRegistry()
	{
		static std::unique_ptr<SubscriptionRegistry<EventExample>> Instance = make_unique<SubscriptionRegistry<EventExample>>();
		return Instance.get();
	}

	friend class SubscriptionRegistry<EventExample>;
};

class ListenerMock
{
public:
	MOCK_METHOD2(beforeEvent, void(const EventBus *, EventExample &));
	MOCK_METHOD2(afterEvent, void(const EventBus *, const EventExample &));
};

class EventBusTest : public Test
{
public:
	EventExample event1;
	EventExample event2;
	EventBus subject1;
	EventBus subject2;
};

TEST_F(EventBusTest, ExecuteNoListeners)
{
	EXPECT_CALL(event1, executeStub(_)).Times(1);
	subject1.executeEvent(event1);
}

TEST_F(EventBusTest, ExecuteIgnoredSubscription)
{
	StrictMock<ListenerMock> listener;

	subject1.subscribeBefore<EventExample>(std::bind(&ListenerMock::beforeEvent, &listener, _1, _2));
	subject1.subscribeAfter<EventExample>(std::bind(&ListenerMock::afterEvent, &listener, _1, _2));

	EXPECT_CALL(listener, beforeEvent(_,_)).Times(0);
	EXPECT_CALL(event1, executeStub(_)).Times(1);
	EXPECT_CALL(listener, afterEvent(_,_)).Times(0);

	subject1.executeEvent(event1);
}

TEST_F(EventBusTest, ExecuteSequence)
{
	StrictMock<ListenerMock> listener1;
	StrictMock<ListenerMock> listener2;

	auto subscription1 = subject1.subscribeBefore<EventExample>(std::bind(&ListenerMock::beforeEvent, &listener1, _1, _2));
	auto subscription2 = subject1.subscribeAfter<EventExample>(std::bind(&ListenerMock::afterEvent, &listener1, _1, _2));
	auto subscription3 = subject1.subscribeBefore<EventExample>(std::bind(&ListenerMock::beforeEvent, &listener2, _1, _2));
	auto subscription4 = subject1.subscribeAfter<EventExample>(std::bind(&ListenerMock::afterEvent, &listener2, _1, _2));

	{
		InSequence local;
		EXPECT_CALL(listener1, beforeEvent(Eq(&subject1), Ref(event1))).Times(1);
		EXPECT_CALL(listener2, beforeEvent(Eq(&subject1), Ref(event1))).Times(1);
		EXPECT_CALL(event1, executeStub(_)).Times(1);
		EXPECT_CALL(listener1, afterEvent(Eq(&subject1), Ref(event1))).Times(1);
		EXPECT_CALL(listener2, afterEvent(Eq(&subject1), Ref(event1))).Times(1);
	}

	subject1.executeEvent(event1);
}

TEST_F(EventBusTest, BusesAreIndependent)
{
	StrictMock<ListenerMock> listener1;
	StrictMock<ListenerMock> listener2;

	auto subscription1 = subject1.subscribeBefore<EventExample>(std::bind(&ListenerMock::beforeEvent, &listener1, _1, _2));
	auto subscription2 = subject1.subscribeAfter<EventExample>(std::bind(&ListenerMock::afterEvent, &listener1, _1, _2));
	auto subscription3 = subject2.subscribeBefore<EventExample>(std::bind(&ListenerMock::beforeEvent, &listener2, _1, _2));
	auto subscription4 = subject2.subscribeAfter<EventExample>(std::bind(&ListenerMock::afterEvent, &listener2, _1, _2));

	EXPECT_CALL(listener1, beforeEvent(_, _)).Times(1);
	EXPECT_CALL(listener2, beforeEvent(_, _)).Times(0);
	EXPECT_CALL(event1, executeStub(_)).Times(1);
	EXPECT_CALL(listener1, afterEvent(_, _)).Times(1);
	EXPECT_CALL(listener2, afterEvent(_, _)).Times(0);

	subject1.executeEvent(event1);
}

}
