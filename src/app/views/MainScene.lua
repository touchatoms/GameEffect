
local MainScene = class("MainScene", cc.load("mvc").ViewBase)

MainScene.RESOURCE_FILENAME = "MainScene.csb"

MainScene.RESOURCE_BINDING = {
    image_bg = {
        varname = "_image_bg",
    },
    node_container = {
        varname = "_node_container",
    }
}

function MainScene:onCreate()
    self._options = {}
    self._data = {}
end

function MainScene:onEnter_()
    display:getRunningScene():getPhysicsWorld():setGravity(cc.p(0, 0))
    self:_initBorder()
    self:_initWall()
    self:_createView()
end

function MainScene:_initWall()
    local size = cc.size(display.width - 4 - 1 , display.width - 4 - 1)
    local wall = cc.Node:create()
    wall:setPosition(cc.p(display.width / 2, display.height / 2))
    wall:setPhysicsBody(cc.PhysicsBody:createEdgeBox(size, cc.PhysicsMaterial(0.1, 0.5, 0.5)))
    self:addChild(wall)

    self.scroContainer = cc.LayerColor:create(cc.c4b(0, 255, 0, 50))
    self.scroContainer:setContentSize(cc.size(100, 100))
    --- create the CCScrollView
end

function MainScene:_initBorder()
    local size = cc.size(display.width - 1 , display.width - 1)
    self._image_bg:setContentSize(size)
    self._image_bg:setColor(cc.c3b(235,235,235))
    self._image_bg:setPosition(cc.p(display.width / 2, display.height / 2))
end

function MainScene:_createView()
    local layer = display.newLayer({ r = 100, g = 100, b = 100, a = 100})
    layer:setContentSize(cc.size(display.width, display.height))
    self:addChild(layer, -1)
    self._options.index = 1
    self:_createMainbar()
    self:_createBox()
end

function MainScene:_createMainbar()
    self._mainbar = self.app_:createView("main.Mainbar", {
        callback = function()
            print('end game')
            local resultDialog = self.app_:createView("main.ResultDialog", {
                score = self._options.index - 1,
                restart = function()
                    self._options.index = 0
                    self:_createBox()
                    self._mainbar:restart()
                end
            }, {})
            resultDialog:open()
        end
    }, {})
    self:addChild(self._mainbar)
end

function MainScene:_createBox()
    if self._box_node then
        self._box_node:removeFromParent()
        self._box_node = nil
        self._options.index = self._options.index + 1
        if not CONFIGS[self._options.index] then
            self._options.index = 1
        end
    end

    print(self._options.index)

    self._mainbar:setScore(self._options.index - 1)
    self._node_container:setPosition(display.width / 2, display.height / 2)
    self._box_node = cc.Node:create()
    local temp_node = self.app_:createView(CONFIGS[self._options.index].URL)

    local index = math.random(1, CONFIGS[self._options.index].COUNT)
    self._boxes = {}
    for i = 1, CONFIGS[self._options.index].COUNT do
        print(i, "index")
        print(CONFIGS[self._options.index].SHAPE, "SHAPE")
        local Square = self.app_:createView(CONFIGS[self._options.index].SHAPE, {
            normal = ConvertHexToRGB(CONFIGS[self._options.index].NORMAL),
            colorRatio = CONFIGS[self._options.index].COLOR_RATIO,
            scale = (CONFIGS[self._options.index].SCALE),
            status = index == i,
            callback = function()
                self:_createBox()
            end
        }, {id = i})
        table.insert(self._boxes, Square)

        local name = string.format("Node_%03d", i)

        local node = temp_node.resourceNode_:getChildByName(name)
        node:addChild(Square)
    end
    self._box_node:addChild(temp_node)
    self._node_container:addChild(self._box_node)
end

return MainScene