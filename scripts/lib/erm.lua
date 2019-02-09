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

local function createReceiverLoader(name)
	local loader = function(x, ...)
		if not Receivers[name] then
			Receivers[name] = require("core:erm."..name)
			Receivers[name].ERM = ERM
		end
		local receiver = Receivers[name]:new(x, ...)
		return receiver
	end
	return loader
end

ERM.BM = createReceiverLoader("BM")
ERM.BU = createReceiverLoader("BU")
ERM.IF = createReceiverLoader("IF")
ERM.MF = createReceiverLoader("MF")

local Triggers = {}

local function createTriggerLoader(name)
	local loader = function(...)
		Triggers[name] = Triggers[name] or require("core:erm."..name.."_T")

		local trigger = Triggers[name]
		trigger.ERM = ERM
		return trigger
	end
	return loader
end

local TriggerLoaders = {}

TriggerLoaders.PI = createTriggerLoader("PI")
TriggerLoaders.MF = createTriggerLoader("MF")


ERM.addTrigger = function(self, t)
	local name = t.name
	local fn = t.fn

	local trigger = TriggerLoaders[name]()

	table.insert(trigger.fn, fn)
end

ERM.callInstructions = function(self, cb)
	if not DATA.ERM.instructionsCalled then
		cb()
		self:callTrigger("PI")
		DATA.ERM.instructionsCalled = true
	end
end

ERM.callTrigger = function(self, name)
	local trigger = TriggerLoaders[name]()
	trigger:call()
end

return ERM
