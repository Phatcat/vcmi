/*
 * UnitProxy.cpp, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

#include "StdInc.h"

#include "UnitProxy.h"

#include "../../LuaStack.h"
#include "../../LuaCallWrapper.h"
#include "../Registry.h"

namespace scripting
{
namespace api
{
namespace battle
{

VCMI_REGISTER_SCRIPT_API(UnitProxy, "battle.Unit")

const std::vector<UnitProxy::RegType> UnitProxy::REGISTER =
{
	{
		"isAlive",
		&UnitProxy::isAlive
	},
	{
		"unitId",
		&UnitProxy::unitId
	}
};

int UnitProxy::isAlive(lua_State * L, const Unit * object)
{
	return LuaCallWrapper<const Unit>::wrap(L, object, &Unit::alive);
}

int UnitProxy::unitId(lua_State * L, const Unit * object)
{
	return LuaCallWrapper<const IUnitInfo>::wrap(L, object, &IUnitInfo::unitId);
}


}
}
}
