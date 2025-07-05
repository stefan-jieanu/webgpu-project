#pragma once

#include <webgpu/webgpu.hpp>
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

namespace gfx
{
    class Adapter
    {
    public:
        Adapter(wgpu::Instance instance, wgpu::Surface surface);
        ~Adapter();

        inline wgpu::Adapter get() { return adapter; }

    private:
        wgpu::Adapter adapter;
    };

} // namespace gfx
