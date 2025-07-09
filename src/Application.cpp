#include "Application.hpp"

Application::Application()
{
    // Create a window
    window = gfx::Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    // Initialize the graphics backend
    if (!bear.initialize(window))
        std::cout << "Aoleu, am belito!\n";

    window.setInputManager(&inputManager);
}

bool Application::isRunning()
{
    return !window.shouldClose();
}

void Application::mainLoop()
{
    window.pollEvents();

    if (inputManager.wasKeyPressed(GLFW_KEY_A))
        std::cout << "pressed a\n";

    if (inputManager.wasKeyReleased(GLFW_KEY_A))
        std::cout << "released a\n";

    if (inputManager.isKeyDown(GLFW_KEY_E))
        std::cout << " is down: e\n";

    // Use in logic:
    if (inputManager.wasMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        std::cout << "Mouse left clicked!\n";
    }

    // Use in logic:
    if (inputManager.wasMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT))
    {
        std::cout << "Mouse left released!\n";
    }

    double scrollX, scrollY;
    inputManager.getScrollOffset(scrollX, scrollY);

    if (scrollY != 0.0)
    {
        std::cout << "Scrolled vertically: " << scrollY << "\n";
    }

    if (scrollX != 0.0)
    {
        std::cout << "Scrolled horizontally: " << scrollX << "\n";
    }

    // double mx, my;
    // inputManager.getMousePosition(mx, my);
    // std::cout << "Mouse position: " << mx << ", " << my << "\n";

    // double dx, dy;
    // inputManager.getMouseDelta(dx, dy);
    // if (dx != 0 || dy != 0)
    // {
    //     std::cout << "Mouse moved: (" << dx << ", " << dy << ")\n";
    // }

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