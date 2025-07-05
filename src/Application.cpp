#include "Application.hpp"

Application::Application()
{
    wgpu::Instance instance = wgpuCreateInstance(nullptr);

    window = gfx::Window(640, 480, "Learn WebGPU");

    surface = glfwGetWGPUSurface(instance, window.get());

    gfx::Adapter adapter = gfx::Adapter(instance, surface);

    instance.release();

    device = gfx::Device(adapter);

    queue = device.get().getQueue();

    // Configure the surface
    wgpu::SurfaceConfiguration config = {};

    // Configuration of the textures created for the underlying swap chain
    config.width = 640;
    config.height = 480;
    config.usage = wgpu::TextureUsage::RenderAttachment;
    wgpu::TextureFormat surfaceFormat = surface.getPreferredFormat(adapter.get());
    config.format = surfaceFormat;

    // And we do not need any particular view format:
    config.viewFormatCount = 0;
    config.viewFormats = nullptr;
    config.device = device.get();
    config.presentMode = wgpu::PresentMode::Fifo;
    config.alphaMode = wgpu::CompositeAlphaMode::Auto;

    surface.configure(config);
}

void Application::mainLoop()
{
    glfwPollEvents();

    // Get the next target texture view
    wgpu::TextureView targetView = getNextSurfaceTextureView();
    if (!targetView)
        return;

    // Create a command encoder for the draw call
    wgpu::CommandEncoderDescriptor encoderDesc = {};
    encoderDesc.label = "My command encoder";
    wgpu::CommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device.get(), &encoderDesc);

    // Create the render pass that clears the screen with our color
    wgpu::RenderPassDescriptor renderPassDesc = {};

    // The attachment part of the render pass descriptor describes the target texture of the pass
    wgpu::RenderPassColorAttachment renderPassColorAttachment = {};
    renderPassColorAttachment.view = targetView;
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
    renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;
    renderPassColorAttachment.clearValue = WGPUColor{0.9, 0.1, 0.2, 1.0};
#ifndef WEBGPU_BACKEND_WGPU
    renderPassColorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif // NOT WEBGPU_BACKEND_WGPU

    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &renderPassColorAttachment;
    renderPassDesc.depthStencilAttachment = nullptr;
    renderPassDesc.timestampWrites = nullptr;

    // Create the render pass and end it immediately (we only clear the screen but do not draw anything)
    wgpu::RenderPassEncoder renderPass = encoder.beginRenderPass(renderPassDesc);
    renderPass.end();
    renderPass.release();

    // Finally encode and submit the render pass
    wgpu::CommandBufferDescriptor cmdBufferDescriptor = {};
    cmdBufferDescriptor.label = "Command buffer";
    wgpu::CommandBuffer command = encoder.finish(cmdBufferDescriptor);
    encoder.release();

    std::cout << "Submitting command..." << std::endl;
    queue.submit(1, &command);
    command.release();
    std::cout << "Command submitted." << std::endl;

    // At the enc of the frame
    targetView.release();
#ifndef __EMSCRIPTEN__
    surface.present();
#endif

#if defined(WEBGPU_BACKEND_DAWN)
    device.get().tick();
#elif defined(WEBGPU_BACKEND_WGPU)
    device.get().poll(false);
#endif
}

bool Application::isRunning()
{
    // return !glfwWindowShouldClose(window);
    return !window.shouldClose();
}

wgpu::TextureView Application::getNextSurfaceTextureView()
{
    // Get the surface texture
    wgpu::SurfaceTexture surfaceTexture;
    surface.getCurrentTexture(&surfaceTexture);
    if (surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::Success)
    {
        return nullptr;
    }
    wgpu::Texture texture = surfaceTexture.texture;

    // Create a view for this surface texture
    wgpu::TextureViewDescriptor viewDescriptor;
    viewDescriptor.label = "Surface texture view";
    viewDescriptor.format = texture.getFormat();
    viewDescriptor.dimension = wgpu::TextureViewDimension::_2D;
    viewDescriptor.baseMipLevel = 0;
    viewDescriptor.mipLevelCount = 1;
    viewDescriptor.baseArrayLayer = 0;
    viewDescriptor.arrayLayerCount = 1;
    viewDescriptor.aspect = wgpu::TextureAspect::All;
    wgpu::TextureView targetView = texture.createView(viewDescriptor);

#ifndef WEBGPU_BACKEND_WGPU
    // We no longer need the texture, only its view
    // (NB: with wgpu-native, surface textures must not be manually released)
    wgpuTextureRelease(surfaceTexture.texture);
#endif // WEBGPU_BACKEND_WGPU

    return targetView;
}

void Application::terminate()
{
    surface.unconfigure();
    queue.release();
    surface.release();
    device.release();
    window.terminate();
}

Application::~Application() {}