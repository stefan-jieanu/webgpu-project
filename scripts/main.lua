function update()
    if wasKeyPressed(Keys.SPACE) then
        print("Space pressed")
    end

    if isKeyDown(Keys.W) then
        print("Moving forward")
    end

    if wasMouseButtonPressed(Mouse.LEFT) then
        print("Left click!")
    end

    local x, y = getMousePosition()
    local dx, dy = getMouseDelta()
    print(string.format("Mouse: %.1f, %.1f | Delta: %.2f, %.2f", x, y, dx, dy))

    local sx, sy = getScrollOffset()
    if sy ~= 0 then
        print("Scrolled Y:", sy)
    end
end