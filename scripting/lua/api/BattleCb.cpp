/*
 * BattleCb.cpp, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#include "StdInc.h"

#include "api/BattleCb.h"

#include "../LuaStack.h"
#include "../LuaCallWrapper.h"

#include "../../../lib/GameConstants.h"

namespace scripting
{
namespace api
{

const std::vector<BattleCbProxy::RegType> BattleCbProxy::REGISTER =
{
	{
		"getBattlefieldType",
		&BattleCbProxy::getBattlefieldType
	},
	{
		"getNextUnitId",
		&BattleCbProxy::getNextUnitId
	},
	{
		"getTacticDistance",
		&BattleCbProxy::getTacticDistance
	},
	{
		"getTerrainType",
		&BattleCbProxy::getTerrainType
	},
	{
		"getUnitByPos",
		&BattleCbProxy::getUnitByPos
	},
	{
		"isFinished",
		&BattleCbProxy::isFinished
	}
};

int BattleCbProxy::getBattlefieldType(lua_State * L, const BattleCb * object)
{
	LuaStack S(L);
	auto ret = object->battleGetBattlefieldType();
	S.push(static_cast<si32>(ret.num));
	return 1;
}

int BattleCbProxy::getNextUnitId(lua_State * L, const BattleCb * object)
{
	return LuaCallWrapper<const BattleCb>::wrap(L, object, &BattleCb::battleNextUnitId);
}

int BattleCbProxy::getTacticDistance(lua_State * L, const BattleCb * object)
{
	return LuaCallWrapper<const BattleCb>::wrap(L, object, &BattleCb::battleTacticDist);
}

int BattleCbProxy::getTerrainType(lua_State * L, const BattleCb * object)
{
	LuaStack S(L);
	auto ret = object->battleTerrainType();
	S.push(static_cast<si32>(ret.num));
	return 1;
}

int BattleCbProxy::getUnitByPos(lua_State * L, const BattleCb * object)
{
	LuaStack S(L);

	BattleHex hex;

	if(!S.tryGet(1, hex.hex))
		return S.retNil();

	bool onlyAlive;

	if(!S.tryGet(2, onlyAlive))
		onlyAlive = true;//same as default value in battleGetUnitByPos

	S.push(object->battleGetUnitByPos(hex, onlyAlive));
	return 1;
}

int BattleCbProxy::isFinished(lua_State * L, const BattleCb * object)
{
	return LuaCallWrapper<const BattleCb>::wrap(L, object, &BattleCb::battleIsFinished);
}

}
}
