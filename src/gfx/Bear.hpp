#include <webgpu/webgpu.hpp>

#include "Window.hpp"

namespace gfx
{
    /*
        Holds all WGPU related data and drawing functions
        -- name subject to change
    */
    class Bear
    {
    public:
        Bear();
        ~Bear();

        // Initialize everything and return true if it went all right
        bool initialize(Window &window);

        // Draw functions
        void draw();

        // Uninitialize everything that was initialized
        void terminate();

    private:
        wgpu::TextureView getNextSurfaceTextureView();

        void initializePipeline();

        // We put here all the variables that are shared between init and main loop
        wgpu::Device device;
        wgpu::Queue queue;
        wgpu::Surface surface;
        std::unique_ptr<wgpu::ErrorCallback> uncapturedErrorCallbackHandle;
        wgpu::TextureFormat surfaceFormat = wgpu::TextureFormat::Undefined;
        wgpu::RenderPipeline pipeline;
    };
} // namespace gfx
