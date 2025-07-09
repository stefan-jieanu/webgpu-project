#include "LuaManager.hpp"
#include <iostream>

LuaManager::LuaManager()
{
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);
}

void LuaManager::loadScript(const std::string &filename)
{
    try
    {
        lua.script_file(filename);
    }
    catch (const sol::error &e)
    {
        std::cerr << "Lua error: " << e.what() << "\n";
    }
}

void LuaManager::update()
{
    sol::function update = lua["update"];
    if (update.valid())
    {
        try
        {
            update();
        }
        catch (const sol::error &e)
        {
            std::cerr << "Lua runtime error in update(): " << e.what() << "\n";
        }
    }
}

sol::state &LuaManager::getState()
{
    return lua;
}
