--
-- Created by IntelliJ IDEA.
-- User: mingzhen.pi
-- Date: 15/8/28
-- Time: 下午1:23
-- To change this template use File | Settings | File Templates.
--
function SetInterval(func, secs)
    local id = cc.Director:getInstance():getScheduler():scheduleScriptFunc(function()
        func()
    end, secs, false)
    return id
end

function ClearInterval(id)
    cc.Director:getInstance():getScheduler():unscheduleScriptEntry(id)
end

function SetTimeout(func, secs)
    local id
    id = cc.Director:getInstance():getScheduler():scheduleScriptFunc(function()
        cc.Director:getInstance():getScheduler():unscheduleScriptEntry(id)
        func()
    end, secs, false)
    return id
end