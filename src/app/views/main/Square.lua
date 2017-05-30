local Square = class("Square", cc.load("mvc").ViewBase)

function Square:onCreate(options, data)
    self._options = options
    self._data = data
    self:setAnchorPoint(cc.p(0.5, 0.5))
    self:_initBgClick()
    self:_initBody()
    self:setScale(self._options.scale)
end

function Square:_initBody()
    self._sprite = display.newSprite("square.png")
    self._sprite:setAnchorPoint(cc.p(0, 0))
    self._sprite:setColor(cc.c3b(unpack(self._options.normal)))
    self._sprite:setTag(10)
    if self._options.status then
        local color = self._options.normal
        local r = color[1] * self._options.colorRatio
        local g = color[2] * self._options.colorRatio
        local b = color[3] * self._options.colorRatio
        print(r, g, b, "color=========")
        self._sprite:setColor(cc.c3b(r, g, b))
    end
    self:addChild(self._sprite)
    self._options._size = self._sprite:getContentSize()
    self:setContentSize(cc.size(self._options._size.width, self._options._size.height))
--    print("square", self._options._size.width, self._options._size.height)
    local size = cc.size(self._options._size.width * self._options.scale, self._options._size.height * self._options.scale)

    local body = cc.PhysicsBody:createBox(self._options._size, cc.PhysicsMaterial(0.1, 1, 0.0))
    self:setPhysicsBody(body)
--    local velocity = cc.p((math.random() - 0.5) * 1000, (math.random() - 0.5) * 1000)
    local velocity = cc.p((math.random() - 0.5) * 200, (math.random() - 0.5) * 200)
    self:getPhysicsBody():setVelocity(velocity)
    self:getPhysicsBody():setCategoryBitmask(1)
    self:getPhysicsBody():setContactTestBitmask(1)
    self:getPhysicsBody():setCollisionBitmask(1)
end

function Square:_initBgClick()
    local btn = self.app_:createView("ui.CCUIButton", {
        bg = {"square.png", "square.png", "square.png"},
        click = function()
            if self._options.callback and self._options.status then
                self._options.callback()
            end
        end
    }, {})
    self:addChild(btn)
end

function Square:_getButtonMoveStatus(pos1, pos2)
    local calX = pos2.x - pos1.x
    local calY = pos2.y - pos1.y
    return math.abs(math.pow((calX * calX + calY * calY), 0.5)) <= 10
end

return Square