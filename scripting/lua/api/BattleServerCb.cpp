/*
 * BattleServerCb.cpp, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

#include "StdInc.h"

#include "BattleServerCb.h"

#include "../LuaStack.h"

#include "Registry.h"

#include "../../../lib/NetPacks.h"

namespace scripting
{
namespace api
{

VCMI_REGISTER_CORE_SCRIPT_API(BattleServerCbProxy);

const std::vector<BattleServerCbProxy::RegType> BattleServerCbProxy::REGISTER =
{
	{
		"addToBattleLog",
		&BattleServerCbProxy::apply<BattleLogMessage>
	},
	{
		"moveUnit",
		&BattleServerCbProxy::apply<BattleStackMoved>
	},
	{
		"changeUnits",
		&BattleServerCbProxy::apply<BattleUnitsChanged>
	}
};

template<typename NetPack>
int BattleServerCbProxy::apply(lua_State * L, ServerBattleCb * object)
{
	LuaStack S(L);

	std::shared_ptr<NetPack> pack;

	if(!S.tryGet(1, pack))
		return S.retVoid();

	object->apply(pack.get());

	return S.retVoid();
}


}
}

