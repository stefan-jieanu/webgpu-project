#include "Application.hpp"

Application::Application()
{
    // Create a window
    window = gfx::Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    // Initialize the graphics backend
    if (!bear.initialize(window))
        std::cout << "Aoleu, am belito!\n";
}

bool Application::isRunning()
{
    return !window.shouldClose();
}

void Application::mainLoop()
{
    window.pollEvents();

    if (window.isKeyPressed(GLFW_KEY_ESCAPE))
        window.close();

    if (window.isKeyPressed(GLFW_KEY_A))
        std::cout << "pressed a\n";

    bear.draw();
}

Application::~Application()
{
    bear.terminate();
}