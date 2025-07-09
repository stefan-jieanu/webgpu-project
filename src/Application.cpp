#include "Application.hpp"

Application::Application()
{
    // Create a window
    window = gfx::Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    // Initialize the graphics backend
    if (!bear.initialize(window))
        std::cout << "Aoleu, am belito!\n";

    window.setInputManager(&inputManager);

    // Register input functions in Lua
    auto &luaState = lua.getState();

    // Keyboard
    luaState.set_function("isKeyDown", [this](int key)
                          { return inputManager.isKeyDown(key); });

    luaState.set_function("wasKeyPressed", [this](int key)
                          { return inputManager.wasKeyPressed(key); });

    luaState.set_function("wasKeyReleased", [this](int key)
                          { return inputManager.wasKeyReleased(key); });

    // Mouse buttons
    luaState.set_function("isMouseButtonDown", [this](int button)
                          { return inputManager.isMouseButtonDown(button); });

    luaState.set_function("wasMouseButtonPressed", [this](int button)
                          { return inputManager.wasMouseButtonPressed(button); });

    luaState.set_function("wasMouseButtonReleased", [this](int button)
                          { return inputManager.wasMouseButtonReleased(button); });

    // Mouse position
    luaState.set_function("getMousePosition", [this]()
                          {
        double x, y;
        inputManager.getMousePosition(x, y);
        return std::make_tuple(x, y); });

    // Mouse delta
    luaState.set_function("getMouseDelta", [this]()
                          {
        double dx, dy;
        inputManager.getMouseDelta(dx, dy);
        return std::make_tuple(dx, dy); });

    // Scroll
    luaState.set_function("getScrollOffset", [this]()
                          {
        double sx, sy;
        inputManager.getScrollOffset(sx, sy);
        return std::make_tuple(sx, sy); });

    // Keyboard keys (GLFW key codes)
    sol::table keys = luaState.create_named_table("Keys");
    keys["SPACE"] = GLFW_KEY_SPACE;
    keys["W"] = GLFW_KEY_W;
    keys["ESCAPE"] = GLFW_KEY_ESCAPE;

    sol::table mouse = luaState.create_named_table("Mouse");
    mouse["LEFT"] = GLFW_MOUSE_BUTTON_LEFT;
    mouse["RIGHT"] = GLFW_MOUSE_BUTTON_RIGHT;

    lua.loadScript("./scripts/main.lua");
}

bool Application::isRunning()
{
    return !window.shouldClose();
}

void Application::mainLoop()
{
    window.pollEvents();

    lua.update();

    if (inputManager.isKeyDown(GLFW_KEY_ESCAPE))
        window.close();

    bear.draw();

    // Update the inputManger at the end of the main loop
    // so that the previous state is set correctly
    inputManager.update();
}

Application::~Application()
{
    bear.terminate();
}