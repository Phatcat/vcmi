require("battle.Unit")
local MF = {}

MF.D = function(self, x, ...)
	return self.ERM.activeEvent:getInitalDamage()
end

MF.E = function(self, x, ...)
	error("MF.E is not implemented")
end

MF.F = function(self, x, p1, ...)
	if p1 then
		self.ERM.activeEvent:setDamage(p1)
		return nil
	else
		return self.ERM.activeEvent:getDamage()
	end
end

MF.N = function(self, x, ...)
	return self.ERM.activeEvent:getTarget():unitId()
end

MF.W = function(self, x, ...)
	error("MF.W is not implemented")
end

return MF
