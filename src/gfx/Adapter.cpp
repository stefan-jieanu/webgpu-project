#include <iostream>
#include <cassert>
#include <vector>

#include "Adapter.hpp"

namespace gfx
{
    Adapter::Adapter(wgpu::Instance instance, wgpu::Surface surface)
    {
        std::cout << "Requesting adapter..." << std::endl;

        wgpu::RequestAdapterOptions adapterOpts = {};
        adapterOpts.compatibleSurface = surface;
        this->adapter = instance.requestAdapter(adapterOpts);

        std::cout << "Got adapter: " << adapter << std::endl;
    }

    Adapter::~Adapter()
    {
        this->adapter.release();
    }
} // namespace gfx
