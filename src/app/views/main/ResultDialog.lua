local ResultDialog = class("ResultDialog", cc.load("mvc").ViewBase)

ResultDialog.RESOURCE_FILENAME = "ResultDialog.csb"

ResultDialog.RESOURCE_BINDING = {
    text_title = {
        varname = "_text_title",
    },
    btn_restart = {
        varname = "_btn_restart",
        events = {
            {
                method = "resart",
                event = "touch"
            }
        }
    },
}

function ResultDialog:onCreate(_options, data)
    self._options = _options
    self._text_title:setString(TITLE[self._options.score or 1].name or "END")
    self._text_title:setColor(TITLE[self._options.score or 1].color or cc.c3b(255, 255, 255))
end

function ResultDialog:open()
    local scene = display:getRunningScene()
    self._content = cc.LayerColor:create(cc.c4b(100, 100, 100, 255))
    self._content:setContentSize(cc.size(display.width, display.height))
    self._content:registerScriptTouchHandler(function(state, ...)
        local args = {...}
        local event = {name = state}
        event.x = args[1]
        event.y = args[2]
    end, false, 0, true)
    self._content:setTouchEnabled(true)
    scene:addChild(self._content)
    self:setPosition(display.width / 2, display.height / 2)
    self._content:addChild(self)
end

function ResultDialog:resart(event)
    if event.name == "ended" then
        if self._options.restart then
            self._options.restart()
            self._content:removeFromParent()
        end
    end
end

return ResultDialog