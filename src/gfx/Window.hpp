#pragma once

#include <string>
#include <unordered_map>

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

        // move constructor to move the GLFWwindow* window pointer
        Window(Window &&other) noexcept;
        Window &operator=(Window &&other) noexcept;

        // delete copy constructor
        // never copy this class, we have one GLFWwindow* that should never be copied
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        inline int getWidth() const { return width; }
        inline int getHeight() const { return height; }
        inline bool shouldClose() const { return glfwWindowShouldClose(window); }

        void close();
        inline GLFWwindow *get() const { return window; }
        inline void pollEvents() const { glfwPollEvents(); }
        void terminate();

        // Input state queries
        bool isKeyPressed(int key) const;
        bool isMouseButtonPressed(int button) const;
        void getMousePosition(double &x, double &y) const;

    private:
        int width, height;
        std::string title;
        GLFWwindow *window;

        static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);

        std::unordered_map<int, bool> keyStates;
        std::unordered_map<int, bool> mouseButtonStates;
        double mouseX = 0.0, mouseY = 0.0;
    };
} // namespace gfx
