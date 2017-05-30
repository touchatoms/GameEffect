---
-- 全屏模糊封装Node
-- Created by IntelliJ IDEA.
-- User: pimingzhen
-- Date: 16/5/11
-- Time: 15:29
--

local NodeScreenBlur = class("NodeScreenBlur", function()
    return cc.Node:create()
end)

--- 构造方法
-- @int gaussianBlurRadius 高斯模糊半径
-- @int scaleFactor 缩放大小大于1的整数:数越大缩的越小
function NodeScreenBlur:ctor(gaussianBlurRadius, scaleFactor)
    local function afterCaptured(succeed, texture)
        dump("Capture screen success.")
        if succeed and self and not tolua.isnull(self) then
            local sp = cc.Sprite:createWithTexture(texture)
            sp:setScaleX(display.width / sp:getContentSize().width)
            sp:setScaleY(display.height / sp:getContentSize().height)
            sp:addTo(self)
            self.sp = sp
            if self.successCallback then
                self.successCallback(self)
            end
        else
            dump("Capture screen failed.")
        end
    end
    xpcall(function()
        dump("Capture screen call.")
        cc.utils:captureScreenBlurTexture(afterCaptured, gaussianBlurRadius or 3, scaleFactor or 4)
    end, __G__TRACKBACK__)
end

function NodeScreenBlur:setSuccessCallback(successCallback)
    self.successCallback = successCallback
end

return NodeScreenBlur