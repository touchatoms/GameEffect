
local IndexScene = class("IndexScene", cc.load("mvc").ViewBase)

IndexScene.RESOURCE_FILENAME = "IndexScene.csb"

IndexScene.RESOURCE_BINDING = {
    text_title = {
        varname = "text_title"
    },
    text_content = {
        varname = "text_content"
    },
    btn_entry = {
        varname = "btn_entry",
        events = {
            {
                method = "buttonTouch",
                event = "touch"
            }
        }
    }
}

function IndexScene:onCreate()
    self.text_title:setVisible(false)
    self.text_content:setVisible(false)
    self.btn_entry:setVisible(false)
    self:createView()
end

function IndexScene:createView()
    local node = cc.Node:create()
    node:setPosition(100, 400)
    self:addChild(node)
    local btn = self.app_:createView("ui.CCUIButton", {
        bg = {"faction_1_active.png", "faction_1.png", "faction1.png"},
        click = function()
        end
    }, {})
    btn:setScale(0.5)
    btn:setPosition(cc.p(0, 145))
    node:addChild(btn)
    local btn = self.app_:createView("ui.CCUIButton", {
        bg = {"faction_2_active.png", "faction_2.png", "faction2.png"},
        click = function()
        end
    }, {})
    btn:setScale(0.5)

    btn:setPosition(cc.p(240, 10))
    node:addChild(btn)
    local btn = self.app_:createView("ui.CCUIButton", {
        bg = {"faction_3_active.png", "faction_3.png", "faction3.png"},
        click = function()
        end
    }, {})
    btn:setScale(0.5)
    btn:setPosition(cc.p(0, 0))
    node:addChild(btn)


    local btn = self.app_:createView("ui.CCUIButton", {
        bg = {"hexagon.png", "hexagon.png", "hexagon.png"},
        click = function()
            local blurNode = require("app.views.ui.NodeScreenBlur").new():addTo(self)
            blurNode:setPosition(display.width / 2, display.height / 2)
        end
    }, {})
    btn:setScale(2)
    btn:setPosition(cc.p(0, -200))
    node:addChild(btn)

end

return IndexScene
