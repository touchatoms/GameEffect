local Mainbar = class("Mainbar", cc.load("mvc").ViewBase)

Mainbar.RESOURCE_FILENAME = "Mainbar.csb"

Mainbar.RESOURCE_BINDING = {
    text_score = {
        varname = "_text_score",
    },
    text_time = {
        varname = "_text_time",
    },
    btn_pause = {
        varname = "_btn_pause",
        events = {
            {
                method = "pause",
                event = "touch"
            }
        }
    },
}

function Mainbar:onCreate(options, data)
    self._options = options
    self._data = {cooldown = 60}
    self:setContentSize(cc.size(display.width, 100))
    self:setAnchorPoint(cc.p(0, 0))
    self:_refreshInterval()
end

function Mainbar:onEnter_()
    print("onEnter_========================")
end

function Mainbar:_refreshInterval()
end

function Mainbar:pause(event)
    if event.name == "ended" then
        local pauseDialog = self.app_:createView("main.PauseDialog", {
            callback = function()
            end
        }, {})
        pauseDialog:open()

        if cc.Director:getInstance():isPaused() then
            cc.Director:getInstance():resume()
        else
            cc.Director:getInstance():pause()
        end
    end
end

function Mainbar:_refreshInterval()
    self._data.cooldown = TIME
    self._text_time:setString(self._data.cooldown)
    if self._interval then
        ClearInterval(self._interval)
        self._interval = nil
    end
    self._interval = SetInterval(function()
        self._data.cooldown = self._data.cooldown - 1
        self._text_time:setString(self._data.cooldown)
        if self._data.cooldown == 0 then
            if self._interval then
                ClearInterval(self._interval)
                self._interval = nil
            end
            if type(self._options.callback) == "function" then
                self._options.callback()
            end
        end
    end, 1)
end

function Mainbar:setScore(score)
    self._text_score:setString("得分:" .. score)
end

function Mainbar:restart()
    self:_refreshInterval()
end

return Mainbar