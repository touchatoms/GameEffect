local Toast = class("Toast", cc.load("mvc").ViewBase)

function Toast:onCreate(_options, data)
    self._options = _options
    self._data = data
    self:_createBg()
    self:_addContent()
end

function Toast:_addContent()
    self.msg = cc.Label:createWithSystemFont(self._data.content, "Arial", 24)
    local size = self.msg:getContentSize()
    if size.width > 380 then
        self.msg:setDimensions(380, 0)
        self.msg:setHorizontalAlignment(cc.TEXT_ALIGNMENT_CENTER)
    end
    self.msg:setColor(cc.c3b(255, 255, 255))
    self.msg:setPosition(size.width / 2 + 10, size.height / 2 + 10)
    self:setContentSize(cc.size(size.width + 20, size.height + 20))
    self._bg:setPreferredSize(cc.size(size.width + 20, size.height + 20))
    self:addChild(self.msg)
    self:setAnchorPoint(cc.p(0.5, 0.5))
end

function Toast:_createBg()
    self._bg = ccui.Scale9Sprite:create("tip_bg.png")
    self._bg:setAnchorPoint(cc.p(0, 0))
    self:addChild(self._bg)
end

function Toast:show()
    self:setVisible(true)
    self.intervalId = SetTimeout(function()
        self:hide()
    end, self._options.duration or 2)
end

function Toast:hide()
    local callback = cc.CallFunc:create(function()
        self:removeFromParent()
        UIUtils.prevToast = nil -- 将prevToast置为nil，下次弹出toast时不做处理
    end)
    local seqAction = cc.Sequence:create(cc.FadeOut:create(self._options.hideDuration or 1), callback)
    self:runAction(seqAction)
--    self.msg:runAction(cc.FadeOut:create(self._options.hideDuration or 1.3))
end

function Toast:refresh()
    local size = self.msg:getContentSize()
    local bgwidth = size.width + 34
    local bgheight = size.height + 23
    self.setPreferredSize(cc.size(bgwidth, bgheight))
    self.msg:setPosition(cc.p(bgwidth / 2, bgheight / 2))
    self.setVisible(false)
end

function Toast:clear()
    self:getActionManager():removeAllActionsFromTarget(self)
    ClearInterval(self.intervalId)
    self:removeFromParent()
    UIUtils.prevToast = nil -- 将prevToast置为nil，下次弹出toast时不做处理
end

return Toast