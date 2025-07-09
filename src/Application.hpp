#include "gfx/Window.hpp"
#include "gfx/Bear.hpp"
#include "InputManager.hpp"

class Application
{
public:
    Application();
    ~Application();

    bool isRunning();
    void mainLoop();

private:
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    const char *WINDOW_TITLE = "Learn WebGPU";

    InputManager inputManager;
    gfx::Window window;
    gfx::Bear bear;
};