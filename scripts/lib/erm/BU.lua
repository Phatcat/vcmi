require("battle.Unit")

local BattleLogMessage = require("netpacks.BattleLogMessage")
local BattleUnitsChanged = require("netpacks.BattleUnitsChanged")

local battle = BATTLE

local BU = {}

local BU_C = function(x, p1)
	assert(type(p1) == "nil", "BU:C can only check value")

	local ret = battle:isFinished()

	if type(ret) == "nil" then
		return 0
	else
		return 1
	end
end

BU.C = function(self, x, ...)
	local argc = select('#', ...)

	if argc == 1 then
		return BU_C(x, ...)
	end
end

local BU_D = function(x, hex, p1)
	assert(type(p1) == "nil", "BU:D can only check value")

	local unit = battle:getUnitByPos(hex, false)

	if unit then
		if unit:isAlive() then
			return nil, -2
		else
			return nil, unit:unitId()
		end
	else
		return nil, -1
	end
end

BU.D = function(self, x, ...)
	local argc = select('#', ...)

	if argc == 2 then
		return BU_D(x, ...)
	end
end

local BU_E = function(x, hex, p1)
	assert(type(p1) == "nil", "BU:E can only check value")

	local unit = battle:getUnitByPos(hex, false)

	if unit and unit:isAlive() then
		return nil, unit:unitId()
	else
		return nil, -1
	end
end

BU.E = function(self, x, ...)
	local argc = select('#', ...)

	if argc == 2 then
		return BU_E(x, ...)
	end
end

local SPECIAL_FIELDS = {}

SPECIAL_FIELDS[0] = 0
SPECIAL_FIELDS[22] = 1
SPECIAL_FIELDS[9] = 2
SPECIAL_FIELDS[18] = 3
SPECIAL_FIELDS[20] = 4
SPECIAL_FIELDS[19] = 5
SPECIAL_FIELDS[17] = 6
SPECIAL_FIELDS[14] = 7
SPECIAL_FIELDS[15] = 8
SPECIAL_FIELDS[16] = 9


local BU_G = function(x, p1)
	assert(type(p1) == "nil", "BU:G? is not implemented")

	local bfield = SPECIAL_FIELDS[battle:getBattlefieldType()]

	if bfield then
		return bfield
	else
		return -1
	end
end

BU.G = function(self, x, ...)
	local argc = select('#', ...)

	if argc == 1 then
		return BU_G(x, ...)
	end
end

local BU_M = function(x, message)
	local pack = BattleLogMessage.new()
	pack:addText(message)
	BATTLESERVER:addToBattleLog(pack)
end

BU.M = function(self, x, ...)
	local argc = select('#', ...)

	if argc == 1 then
		return BU_M(x, ...)
	end
end

local BU_S = function(x, typ, count, hex, side, slot)
	local pack = BattleUnitsChanged.new()

	local id = battle:getNextUnitId()

	pack:add(id,
	{
		newUnitInfo =
		{
			["count"] = count,
			["type"] = typ,
			["side"] = side,
			["position"] = hex,
			["summoned"] = (slot == -1),
		}
	})

	BATTLESERVER:changeUnits(pack)
end

BU.S = function(self, x, ...)
	local argc = select('#', ...)

	if argc >= 5 then
		return BU_S(x, ...)
	end
end

local BU_T = function(x)
	local tacticDistance = battle:getTacticDistance()

	if tacticDistance == 0 then
		return 0
	else
		return 1
	end
end

BU.T = function(self, x, ...)
	local argc = select('#', ...)

	if argc == 1 then
		return BU_T(x, ...)
	end
end

return BU
