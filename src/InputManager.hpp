#pragma once

#include <unordered_map>
#include <iostream>

class InputManager
{
public:
    void update(); // Call once per frame

    // Keyboard
    void setKeyState(int key, bool isDown);
    bool isKeyDown(int key) const;
    bool wasKeyPressed(int key) const;
    bool wasKeyReleased(int key) const;

    // Mouse buttons
    void setMouseButtonState(int button, bool isDown);
    bool isMouseButtonDown(int button) const;
    bool wasMouseButtonPressed(int button) const;
    bool wasMouseButtonReleased(int button) const;

    // Mouse position
    void setMousePosition(double x, double y);
    void getMousePosition(double &x, double &y) const;
    void getMouseDelta(double &dx, double &dy) const;

    // Scroll wheel
    void setScrollOffset(double x, double y);
    void getScrollOffset(double &x, double &y) const;

private:
    std::unordered_map<int, bool> currentKeyStates;
    std::unordered_map<int, bool> previousKeyStates;

    // Mouse buttons
    std::unordered_map<int, bool> currentMouseButtonStates;
    std::unordered_map<int, bool> previousMouseButtonStates;

    // Mouse position
    double mouseX = 0.0, mouseY = 0.0;
    double lastMouseX = 0.0, lastMouseY = 0.0;

    // Scroll wheel
    double scrollX = 0.0;
    double scrollY = 0.0;
};
