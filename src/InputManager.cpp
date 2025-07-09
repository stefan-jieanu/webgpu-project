#include "InputManager.hpp"

void InputManager::update()
{
    previousKeyStates = currentKeyStates;
    previousMouseButtonStates = currentMouseButtonStates;

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    scrollX = 0.0;
    scrollY = 0.0;
}

// --------------------
// Keyboard
// --------------------

void InputManager::setKeyState(int key, bool isDown)
{
    currentKeyStates[key] = isDown;
}

bool InputManager::isKeyDown(int key) const
{
    auto it = currentKeyStates.find(key);
    return it != currentKeyStates.end() && it->second;
}

bool InputManager::wasKeyPressed(int key) const
{
    bool wasDown = false;
    auto it = previousKeyStates.find(key);
    if (it != previousKeyStates.end())
    {
        wasDown = it->second;
    }
    return isKeyDown(key) && !wasDown;
}

bool InputManager::wasKeyReleased(int key) const
{
    bool wasDown = false;
    auto it = previousKeyStates.find(key);
    if (it != previousKeyStates.end())
    {
        wasDown = it->second;
    }

    return !isKeyDown(key) && wasDown;
}

// --------------------
// Mouse buttons
// --------------------

void InputManager::setMouseButtonState(int button, bool isDown)
{
    currentMouseButtonStates[button] = isDown;
}

bool InputManager::isMouseButtonDown(int button) const
{
    auto it = currentMouseButtonStates.find(button);
    return it != currentMouseButtonStates.end() && it->second;
}

bool InputManager::wasMouseButtonPressed(int button) const
{
    bool was = previousMouseButtonStates.count(button) ? previousMouseButtonStates.at(button) : false;
    return isMouseButtonDown(button) && !was;
}

bool InputManager::wasMouseButtonReleased(int button) const
{
    bool was = previousMouseButtonStates.count(button) ? previousMouseButtonStates.at(button) : false;
    return !isMouseButtonDown(button) && was;
}

// --------------------
// Mouse position
// --------------------

void InputManager::setMousePosition(double x, double y)
{
    mouseX = x;
    mouseY = y;
}

void InputManager::getMousePosition(double &x, double &y) const
{
    x = mouseX;
    y = mouseY;
}

void InputManager::getMouseDelta(double &dx, double &dy) const
{
    dx = mouseX - lastMouseX;
    dy = mouseY - lastMouseY;
}

// --------------------
// Scroll wheel
// --------------------

void InputManager::setScrollOffset(double x, double y)
{
    scrollX = x;
    scrollY = y;
}

void InputManager::getScrollOffset(double &x, double &y) const
{
    x = scrollX;
    y = scrollY;
}