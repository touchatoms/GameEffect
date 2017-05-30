local PauseDialog = class("PauseDialog", cc.load("mvc").ViewBase)

PauseDialog.RESOURCE_FILENAME = "PauseDialog.csb"

PauseDialog.RESOURCE_BINDING = {
    btn_resume = {
        varname = "_btn_resume",
        events = {
            {
                method = "resume",
                event = "touch"
            }
        }
    },
}

function PauseDialog:onCreate(_options, data)
end

function PauseDialog:open()
    local scene = display:getRunningScene()
    self._content = cc.LayerColor:create(cc.c4b(100, 100, 100, 255))
    self._content:setContentSize(cc.size(display.width, display.height))
    self._content:registerScriptTouchHandler(function(state, ...)
        local args = { ... }
        local event = { name = state }
        event.x = args[1]
        event.y = args[2]
    end, false, 0, true)
    self._content:setTouchEnabled(true)
    scene:addChild(self._content)
    self:setPosition(display.width / 2, display.height / 2)
    self._content:addChild(self)
end

function PauseDialog:resume(event)
    if event.name == "ended" then
        self._content:removeFromParent()
        cc.Director:getInstance():resume()
    end
end

return PauseDialog