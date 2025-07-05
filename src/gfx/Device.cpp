#include "Device.hpp"

namespace gfx
{
    Device::Device() {}

    Device::Device(Adapter &adapter)
    {
        std::cout << "Requesting device..." << std::endl;
        wgpu::DeviceDescriptor deviceDesc = {};
        deviceDesc.label = "My Device";
        deviceDesc.requiredFeatureCount = 0;
        deviceDesc.requiredLimits = nullptr;
        deviceDesc.defaultQueue.nextInChain = nullptr;
        deviceDesc.defaultQueue.label = "The default queue";
        deviceDesc.deviceLostCallback = [](WGPUDeviceLostReason reason, char const *message, void * /* pUserData */)
        {
            std::cout << "Device lost: reason " << reason;
            if (message)
                std::cout << " (" << message << ")";
            std::cout << std::endl;
        };
        device = adapter.get().requestDevice(deviceDesc);
        std::cout << "Got device: " << device << std::endl;

        uncapturedErrorCallbackHandle = device.setUncapturedErrorCallback(
            [](wgpu::ErrorType type, char const *message)
            {
                std::cout << "Uncaptured device error: type " << type;
                if (message)
                    std::cout << " (" << message << ")";
                std::cout << std::endl;
            });
    }

    Device::~Device()
    {
    }
} // namespace gfx
