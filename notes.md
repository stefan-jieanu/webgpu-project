# Segmentation fault when using our window class with GLFWindow
- our window class has two contructors, one being emtpy
- declaring our window in the Application class uses default one
- when reassigning the window in the AppInit function it calls the destructor on the old one which call glfwDestroyWindow() leading to weird behavior
- FIX (for now): implement a terminate function and destroy the GLFWwindow manully