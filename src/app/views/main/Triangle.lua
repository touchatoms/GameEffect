local Pentagon = class("Pentagon", cc.load("mvc").ViewBase)

function Pentagon:onCreate(options, data)
    self._options = options
    self._data = data
    self:setAnchorPoint(cc.p(0.5, 0.5))
    self:_initBgClick()
    self:_initBody()
    self:setScale(self._options.scale or 1)
end

function Pentagon:_initBody()
    self._sprite = display.newSprite("triangle.png")

    self._sprite:setAnchorPoint(cc.p(0, 0))
    self._sprite:setColor(cc.c3b(unpack(self._options.normal)))
    if self._options.status then
        local color = self._options.normal

        local r = color[1] * self._options.colorRatio
        local g = color[2] * self._options.colorRatio
        local b = color[3] * self._options.colorRatio
        self._sprite:setColor(cc.c3b(r, g, b))
    end
    self:addChild(self._sprite)
    self._options._size = self._sprite:getContentSize()
    self:setContentSize(cc.size(self._options._size.width, self._options._size.height))
    print("triangle", self._options._size.width, self._options._size.height)

    local vers = { cc.p(-50, -43.5), cc.p(0, 43.5), cc.p(50, -43.5)}
    local body = cc.PhysicsBody:createPolygon(vers, cc.PhysicsMaterial(0.1, 1, 0.0))
    self:setPhysicsBody(body)
    local velocity = cc.p((math.random() - 0.5) * 200, (math.random() - 0.5) * 200)
    self:getPhysicsBody():setVelocity(velocity)
    self:getPhysicsBody():setCategoryBitmask(1)
    self:getPhysicsBody():setContactTestBitmask(1)
    self:getPhysicsBody():setCollisionBitmask(1)
end

function Pentagon:_initBgClick()
    local btn = self.app_:createView("ui.CCUIButton", {
        bg = {"triangle.png", "triangle.png", "triangle.png"},
        click = function()
            if self._options.callback and self._options.status then
                self._options.callback()
            end
        end
    }, {})
    self:addChild(btn)
end

function Pentagon:_getButtonMoveStatus(pos1, pos2)
    local calX = pos2.x - pos1.x
    local calY = pos2.y - pos1.y
    return math.abs(math.pow((calX * calX + calY * calY), 0.5)) <= 10
end

return Pentagon