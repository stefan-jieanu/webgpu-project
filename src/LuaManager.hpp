#pragma once
#include <sol/sol.hpp>

class LuaManager
{
public:
    LuaManager();
    void loadScript(const std::string &filename);
    void update();

    sol::state &getState(); // for exposing external functions

private:
    sol::state lua;
};
