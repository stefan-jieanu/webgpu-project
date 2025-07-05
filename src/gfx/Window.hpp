#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

namespace gfx
{
    class Window
    {
    public:
        Window();
        Window(int width, int height, std::string title);
        ~Window();

        inline GLFWwindow *get() { return window; }
        inline bool shouldClose() { return glfwWindowShouldClose(window); }
        inline void pollEvents() { glfwPollEvents(); }
        void terminate();

    private:
        int width, height;
        std::string title;
        GLFWwindow *window;
    };
} // namespace gfx
