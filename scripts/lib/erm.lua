
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
ERM.MF = getReceiverLoader("MF")

local Triggers = {}

local function getTriggerLoader(name)
	local name = name

	local loader = function(...)
		Triggers[name] = Triggers[name] or require("core:erm."..name.."_T")

		local trigger = Triggers[name]
		trigger.ERM = ERM
		return trigger
	end
	return loader
end

local TriggerLoaders = {}

TriggerLoaders.PI = getTriggerLoader("PI")
TriggerLoaders.MF = getTriggerLoader("MF")


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
