local CCUIButton = class("CCUIButton", cc.load("mvc").ViewBase)

function CCUIButton:onCreate(_options, data)
    self._options = _options
    self._data = data
    self:_createButton()
end

function CCUIButton:_createButton()
    local bg_normal = self._options.bg[1]
    local bg_hilight = self._options.bg[2]
    local bg_disable = self._options.bg[3] or self._options.bg[1]
    local btn = lever.LeverVisibleButton:create(bg_normal, bg_hilight, bg_disable, ccui.TextureResType.localType)
    btn:setAnchorPoint(0, 0)
    btn:setTouchEnabled(true)
    btn:setScale9Enabled(true)
    if self._options.preferredSize then
        btn:setContentSize(self._options.preferredSize)
    end
    btn:setSwallowTouches(self._options.swallow or false)
    local isMoved
    btn:addTouchEventListener(function(target, eventType)
        if eventType == ccui.TouchEventType.began then
            isMoved = false
        elseif eventType == ccui.TouchEventType.moved then
            local startPos = target:getTouchBeganPosition()
            local movePos = target:getTouchMovePosition()
            if not self:_getButtonMoveStatus(startPos, movePos) then
                isMoved = true
            end
        elseif eventType == ccui.TouchEventType.ended then
            if not isMoved then
                if self._disable then
                    self:_disableClick()
                else
                    self:_click()
                end
            end
        elseif eventType == ccui.TouchEventType.canceled then
        end
    end)
    self:addChild(btn)
end

function CCUIButton:_click()
    self._options:click(self)
end

function CCUIButton:_disableClick()
    self._options:disableClick(self)
end

function CCUIButton:_getButtonMoveStatus(pos1, pos2)
    local calX = pos2.x - pos1.x
    local calY = pos2.y - pos1.y
    return math.abs(math.pow((calX * calX + calY * calY), 0.5)) <= 10
end

return CCUIButton
