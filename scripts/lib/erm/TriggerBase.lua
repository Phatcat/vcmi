local TriggerBase =
{
	create = function()
		local ret =
		{
			fn = {},
			--y = {}
			--e = {}
			call = function(self, event)
				self.ERM.activeEvent = event
				self.ERM.activeTrigger = self
				for _, fn in ipairs(self.fn) do
					fn()
				end
				self.ERM.activeTrigger = nil
				self.ERM.activeEvent = nil
			end
		}
		return ret
	end
}

return TriggerBase
