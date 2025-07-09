#include "Window.hpp"

#include <iostream>

namespace gfx
{
    Window::Window() {}

    Window::Window(int width, int height, std::string title) : width(width), height(height), title(title)
    {
        // Open window
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, "Learn WebGPU", nullptr, nullptr);

        // Set user pointer to this instance so callbacks can access it
        glfwSetWindowUserPointer(window, this);

        // glfwMakeContextCurrent(window);

        // Register callbacks
        glfwSetKeyCallback(window, keyCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetCursorPosCallback(window, cursorPosCallback);
        glfwSetScrollCallback(window, scrollCallback);
    }

    Window::Window(Window &&other) noexcept
        : width(other.width),
          height(other.height),
          title(std::move(other.title)),
          window(other.window),
          keyStates(std::move(other.keyStates)),
          mouseButtonStates(std::move(other.mouseButtonStates)),
          mouseX(other.mouseX),
          mouseY(other.mouseY)
    {
        // Transfer user pointer to new instance
        if (window)
        {
            glfwSetWindowUserPointer(window, this);
        }

        // Nullify source
        other.window = nullptr;
    }

    Window &Window::operator=(Window &&other) noexcept
    {
        if (this != &other)
        {
            // Clean up current window
            if (window)
                glfwDestroyWindow(window);

            width = other.width;
            height = other.height;
            title = std::move(other.title);
            window = other.window;
            keyStates = std::move(other.keyStates);
            mouseButtonStates = std::move(other.mouseButtonStates);
            mouseX = other.mouseX;
            mouseY = other.mouseY;

            if (window)
                glfwSetWindowUserPointer(window, this);

            other.window = nullptr;
        }
        return *this;
    }

    Window::~Window() {}

    void Window::close()
    {
        if (window)
            glfwSetWindowShouldClose(window, true);
    }

    void Window::terminate()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool Window::isKeyPressed(int key) const
    {
        auto it = keyStates.find(key);
        return it != keyStates.end() && it->second;
    }

    bool Window::isMouseButtonPressed(int button) const
    {
        auto it = mouseButtonStates.find(button);
        return it != mouseButtonStates.end() && it->second;
    }

    void Window::getMousePosition(double &x, double &y) const
    {
        x = mouseX;
        y = mouseY;
    }

    void Window::setInputManager(InputManager *input)
    {
        inputManager = input;
    }

    void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
        if (!win)
            return;

        if (action == GLFW_PRESS)
            win->inputManager->setKeyState(key, true);
        else if (action == GLFW_RELEASE)
            win->inputManager->setKeyState(key, false);
    }

    void Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
    {
        Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
        if (!win)
            return;

        if (action == GLFW_PRESS)
            win->inputManager->setMouseButtonState(button, true);
        else if (action == GLFW_RELEASE)
            win->inputManager->setMouseButtonState(button, false);
    }

    void Window::cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
    {
        Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
        if (!win)
            return;

        win->inputManager->setMousePosition(xpos, ypos);
    }

    void Window::scrollCallback(GLFWwindow *glfwWindow, double xoffset, double yoffset)
    {
        Window *win = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
        if (!win || !win->inputManager)
            return;

        win->inputManager->setScrollOffset(xoffset, yoffset);
    }
} // namespace gfx
