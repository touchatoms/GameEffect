#include "lua_lever_visible_button_auto.hpp"
#include "LeverVisibleButton.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"



int lua_lever_visible_button_LeverVisibleButton_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1 || defined(FORCE_LUABIND_CHECKING)
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1 || defined(FORCE_LUABIND_CHECKING)
    if (!tolua_isusertable(tolua_S,1,"LeverVisibleButton",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S)-1;

    do 
    {
        if (argc == 1)
        {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LeverVisibleButton:create");
            if (!ok) { break; }
            LeverVisibleButton* ret = LeverVisibleButton::create(arg0);
            object_to_luaval<LeverVisibleButton>(tolua_S, "LeverVisibleButton",(LeverVisibleButton*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 2)
        {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LeverVisibleButton:create");
            if (!ok) { break; }
            std::string arg1;
            ok &= luaval_to_std_string(tolua_S, 3,&arg1, "LeverVisibleButton:create");
            if (!ok) { break; }
            LeverVisibleButton* ret = LeverVisibleButton::create(arg0, arg1);
            object_to_luaval<LeverVisibleButton>(tolua_S, "LeverVisibleButton",(LeverVisibleButton*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 3)
        {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LeverVisibleButton:create");
            if (!ok) { break; }
            std::string arg1;
            ok &= luaval_to_std_string(tolua_S, 3,&arg1, "LeverVisibleButton:create");
            if (!ok) { break; }
            std::string arg2;
            ok &= luaval_to_std_string(tolua_S, 4,&arg2, "LeverVisibleButton:create");
            if (!ok) { break; }
            LeverVisibleButton* ret = LeverVisibleButton::create(arg0, arg1, arg2);
            object_to_luaval<LeverVisibleButton>(tolua_S, "LeverVisibleButton",(LeverVisibleButton*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 4)
        {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LeverVisibleButton:create");
            if (!ok) { break; }
            std::string arg1;
            ok &= luaval_to_std_string(tolua_S, 3,&arg1, "LeverVisibleButton:create");
            if (!ok) { break; }
            std::string arg2;
            ok &= luaval_to_std_string(tolua_S, 4,&arg2, "LeverVisibleButton:create");
            if (!ok) { break; }
            cocos2d::ui::Widget::TextureResType arg3;
            ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3, "LeverVisibleButton:create");
            if (!ok) { break; }
            LeverVisibleButton* ret = LeverVisibleButton::create(arg0, arg1, arg2, arg3);
            object_to_luaval<LeverVisibleButton>(tolua_S, "LeverVisibleButton",(LeverVisibleButton*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 0)
        {
            LeverVisibleButton* ret = LeverVisibleButton::create();
            object_to_luaval<LeverVisibleButton>(tolua_S, "LeverVisibleButton",(LeverVisibleButton*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d", "LeverVisibleButton:create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1 || defined(FORCE_LUABIND_CHECKING)
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_lever_visible_button_LeverVisibleButton_create'.",&tolua_err);
#endif
    return 0;
}
int lua_lever_visible_button_LeverVisibleButton_constructor(lua_State* tolua_S)
{
    int argc = 0;
    LeverVisibleButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1 || defined(FORCE_LUABIND_CHECKING)
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj = new LeverVisibleButton();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"LeverVisibleButton");
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "LeverVisibleButton:LeverVisibleButton",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1 || defined(FORCE_LUABIND_CHECKING)
    tolua_error(tolua_S,"#ferror in function 'lua_lever_visible_button_LeverVisibleButton_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_lever_visible_button_LeverVisibleButton_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (LeverVisibleButton)");
    return 0;
}

int lua_register_lever_visible_button_LeverVisibleButton(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"LeverVisibleButton");
    tolua_cclass(tolua_S,"LeverVisibleButton","LeverVisibleButton","ccui.Button",nullptr);

    tolua_beginmodule(tolua_S,"LeverVisibleButton");
        tolua_function(tolua_S,"new",lua_lever_visible_button_LeverVisibleButton_constructor);
        tolua_function(tolua_S,"create", lua_lever_visible_button_LeverVisibleButton_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(LeverVisibleButton).name();
    g_luaType[typeName] = "LeverVisibleButton";
    g_typeCast["LeverVisibleButton"] = "LeverVisibleButton";
    return 1;
}
TOLUA_API int register_all_lever_visible_button(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"lever",0);
	tolua_beginmodule(tolua_S,"lever");

	lua_register_lever_visible_button_LeverVisibleButton(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

