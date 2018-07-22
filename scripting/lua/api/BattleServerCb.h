/*
 * BattleServerCb.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

#pragma once

#include <vcmi/scripting/Service.h>
#include "../../../lib/battle/IBattleEventRealizer.h"

#include "../LuaWrapper.h"

namespace scripting
{
namespace api
{

class BattleServerCbProxy : public OpaqueWrapper<ServerBattleCb, BattleServerCbProxy>
{
public:
	using Wrapper = OpaqueWrapper<ServerBattleCb, BattleServerCbProxy>;

	static const std::vector<typename Wrapper::RegType> REGISTER;

	template<typename NetPack>
	static int apply(lua_State * L, ServerBattleCb * object);
};

}
}
