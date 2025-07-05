// Include the C++ wrapper instead of the raw header(s)
#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>

#include "Application.hpp"

int main()
{
    Application app;

#ifdef __EMSCRIPTEN__
    // Equivalent of the main loop when using Emscripten:
    auto callback = [](void *arg)
    {
        Application *pApp = reinterpret_cast<Application *>(arg);
        pApp->MainLoop(); // 4. We can use the application object
    };
    emscripten_set_main_loop_arg(callback, &app, 0, true);
#else  // __EMSCRIPTEN__
    while (app.isRunning())
    {
        app.mainLoop();
    }
#endif // __EMSCRIPTEN__

    return 0;
}