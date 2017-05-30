local Hexagon = class("Hexagon", cc.load("mvc").ViewBase)

function Hexagon:onCreate(options, data)
    self._options = options
    self._data = data
    self:setAnchorPoint(cc.p(0.5, 0.5))
    self:_initBgClick()
    self:_initBody()
    self:setScale(self._options.scale or 1)
end

function Hexagon:_initBody()
    self._sprite = display.newSprite("hexagon.png")
    self._sprite:setAnchorPoint(cc.p(0, 0))
    self._sprite:setTag(10)
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
    print("hexagon", self._options._size.width, self._options._size.height)
    local vers = {
        cc.p(50, 115),
        cc.p(100, 87),
        cc.p(100, 30),
        cc.p(50, 0),
        cc.p(0, 29),
        cc.p(0, 85)
    }

    for i, ver in ipairs(vers) do
        ver.x = ver.x - 50
        ver.y = ver.y - 57.5
    end
    local body = cc.PhysicsBody:createPolygon(vers, cc.PhysicsMaterial(0.1, 1, 0.0))
    self:setPhysicsBody(body)
    local velocity = cc.p((math.random() - 0.5) * 200, (math.random() - 0.5) * 200)
    self:getPhysicsBody():setVelocity(velocity)
    self:getPhysicsBody():setCategoryBitmask(1)
    self:getPhysicsBody():setContactTestBitmask(1)
    self:getPhysicsBody():setCollisionBitmask(1)
end

function Hexagon:_initBgClick()
    local btn = self.app_:createView("ui.CCUIButton", {
        bg = {"hexagon.png", "hexagon.png", "hexagon.png"},
        click = function()
            if self._options.callback and self._options.status then
                self._options.callback()
            end
        end
    }, {})
    self:addChild(btn)
end

function Hexagon:_getButtonMoveStatus(pos1, pos2)
    local calX = pos2.x - pos1.x
    local calY = pos2.y - pos1.y
    return math.abs(math.pow((calX * calX + calY * calY), 0.5)) <= 10
end

return Hexagon