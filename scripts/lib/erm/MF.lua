local MF = {}

local MF_D = function(x)
	return MF.ERM.activeEvent:getInitalDamage()
end

MF.D = function(self, x, ...)
	local argc = select('#', ...)

	if argc == 1 then
		return MF_D(x, ...)
	else
		error("MF:D requires one argument")
	end
end

local MF_F = function(x, p1)
	if p1 then
		MF.ERM.activeEvent:setDamage(p1)
		return nil
	else
		return MF.ERM.activeEvent:getDamage()
	end
end

MF.F = function(self, x, ...)
	local argc = select('#', ...)

	if argc == 1 then
		return MF_F(x, ...)
	else
		error("MF:F requires one argument")
	end
end

return MF
