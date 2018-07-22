
DATA = DATA or {}

local ERM = {}
local DATA = DATA

DATA.ERM = DATA.ERM or {}

DATA.ERM.flag = DATA.ERM.flag or {}
DATA.ERM.quick = DATA.ERM.quick or {}
DATA.ERM.v = DATA.ERM.v or {}
DATA.ERM.z = DATA.ERM.z or {}

ERM.flag = DATA.ERM.flag
ERM.quick = DATA.ERM.quick
ERM.v = DATA.ERM.v
ERM.z = DATA.ERM.z

local y = {}

ERM.getY = function(key)
	y[key] = y[key] or {}
	return y[key]
end

local Receivers = {}

local function getReceiverLoader(name)
	local name = name

	local loader = function(...)
		Receivers[name] = Receivers[name] or require("core:erm."..name)

		local receiver = Receivers[name]
		receiver.ERM = ERM
		return receiver
	end
	return loader
end

ERM.BU = getReceiverLoader("BU")
ERM.IF = getReceiverLoader("IF")

local triggers = {}

ERM.addTrigger = function(t)
	triggers[t.name] = triggers[t.name] or {}
	table.insert(triggers[t.name], t.fn)
end

ERM.callInstructions = function(cb)
	if not DATA.ERM.instructionsCalled then
		cb()
		ERM.callTrigger("PI")
		DATA.ERM.instructionsCalled = true
	end

end

ERM.callTrigger = function(name)

	if triggers[name] then
		for _, fn in ipairs(triggers[name]) do
			fn()
		end

	end

end

return ERM
