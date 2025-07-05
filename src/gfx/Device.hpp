#include <webgpu/webgpu.hpp>

#include "Adapter.hpp"

namespace gfx
{
    class Device
    {
    public:
        Device();
        Device(Adapter &adapter);
        ~Device();

        // Disable copy
        Device(const Device &) = delete;
        Device &operator=(const Device &) = delete;

        // Enable move
        Device(Device &&) noexcept = default;
        Device &operator=(Device &&) noexcept = default;

        inline wgpu::Device get() { return device; }
        inline void release() { device.release(); }

    private:
        wgpu::Device device;
        std::unique_ptr<wgpu::ErrorCallback> uncapturedErrorCallbackHandle;
    };
} // namespace gfx
