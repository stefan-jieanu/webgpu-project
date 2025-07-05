#include <webgpu/webgpu.hpp>

#include "gfx/Adapter.hpp"
#include "gfx/Device.hpp"
#include "gfx/Window.hpp"

class Application
{
public:
    Application();
    ~Application();

    // Uninitialize everything that was initialized
    void terminate();

    // Draw a frame and handle events
    void mainLoop();

    // Return true as long as the main loop should keep on running
    bool isRunning();

private:
    wgpu::TextureView getNextSurfaceTextureView();

    // We put here all the variables that are shared between init and main loop
    // GLFWwindow *window;
    gfx::Window window;
    gfx::Device device;
    wgpu::Queue queue;
    wgpu::Surface surface;
    std::unique_ptr<wgpu::ErrorCallback> uncapturedErrorCallbackHandle;
};