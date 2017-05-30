--
-- Created by IntelliJ IDEA.
-- User: mingzhen.pi
-- Date: 15/8/28
-- Time: 下午1:23
-- To change this template use File | Settings | File Templates.
--

UIUtils = {}

function UIUtils:ToastShow(content, height, time, hideDuration, app)
        if UIUtils.prevToast then
            UIUtils.prevToast:clear()
        end

        local toast = app:createView("ui.Toast", {
            time = time,
            hideDuration = hideDuration,
            height = height
        }, {
            content = content
        })
        local scene = cc.Director:getInstance():getRunningScene()
        if height then
            toast:setPosition(display.width / 2, height)
        else
            toast:setPosition(display.width / 2, display.height / 2)
        end
        scene:addChild(toast)
        toast:show()
        UIUtils.prevToast = toast
end

