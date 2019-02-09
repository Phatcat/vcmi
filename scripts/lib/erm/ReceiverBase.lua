local ReceiverBase = {}

function ReceiverBase:new()
	o = {}
	setmetatable(o, self)
	self.__index = self
	return o
end

return ReceiverBase

