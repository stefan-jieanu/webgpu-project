#include "gfx/Window.hpp"
#include "gfx/Bear.hpp"

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

    gfx::Window window;
    gfx::Bear bear;
};