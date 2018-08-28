/*
 * ERM_MF.cpp, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

#include "StdInc.h"

#include "../scripting/ScriptFixture.h"
#include "../JsonComparer.h"

#include <vcmi/events/ApplyDamage.h>

#include "../../lib/NetPacks.h"
#include "../../lib/events/EventBus.h"


namespace test
{
namespace scripting
{
using namespace ::testing;
using ::events::ApplyDamage;
using ::events::EventBus;

template <typename T>
class ListenerMock
{
public:
	MOCK_METHOD2_T(beforeEvent, void(const EventBus *, T &));
	MOCK_METHOD2_T(afterEvent, void(const EventBus *, const T &));
};

class ERM_MF : public Test, public ScriptFixture
{
public:
	events::EventBus eventBus;

	StrictMock<ListenerMock<ApplyDamage>> listenerMock;

	std::shared_ptr<StrictMock<UnitMock>> targetMock;

	void setDefaultExpectations()
	{
		EXPECT_CALL(environmentMock, eventBus()).WillRepeatedly(Return(&eventBus));
	}

protected:
	void SetUp() override
	{
		ScriptFixture::setUp();

		targetMock = std::make_shared<StrictMock<UnitMock>>();
	}
};

TEST_F(ERM_MF, ChangeDamage)
{
	std::stringstream source;
	source << "VERM" << std::endl;
	source << "!?MF1;" << std::endl;
	source << "!!MF:D?y-1;" << std::endl;
	source << "!!VRy-1:+10;" << std::endl;
	source << "!!MF:Fy-1;" << std::endl;

	loadScript(VLC->scriptHandler->erm, source.str());
	SCOPED_TRACE("\n" + subject->code);
	run();

	BattleStackAttacked pack;

	std::shared_ptr<ApplyDamage> ev = std::make_shared<ApplyDamage>(&pack, targetMock);

	ev->setDamage(23450);

	eventBus.executeEvent(*ev.get());

	EXPECT_EQ(ev->getDamage(), 23460);
}

}
}

