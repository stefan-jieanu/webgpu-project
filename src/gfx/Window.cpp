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
    }

    Window::~Window() {}

    void Window::terminate()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
} // namespace gfx
