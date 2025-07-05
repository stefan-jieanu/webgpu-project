#include "Bear.hpp"

// We embbed the source of the shader module here
const char *shaderSource = R"(
@vertex
fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f {
	var p = vec2f(0.0, 0.0);
	if (in_vertex_index == 0u) {
		p = vec2f(-0.5, -0.5);
	} else if (in_vertex_index == 1u) {
		p = vec2f(0.5, -0.5);
	} else {
		p = vec2f(0.0, 0.5);
	}
	return vec4f(p, 0.0, 1.0);
}

@fragment
fn fs_main() -> @location(0) vec4f {
	return vec4f(0.0, 0.4, 1.0, 1.0);
}
)";

namespace gfx
{
    Bear::Bear() {}
    Bear::~Bear() {}

    bool Bear::initialize(Window &window)
    {
        wgpu::Instance instance = wgpuCreateInstance(nullptr);

        surface = glfwGetWGPUSurface(instance, window.get());

        std::cout << "Requesting adapter..." << std::endl;
        surface = glfwGetWGPUSurface(instance, window.get());
        wgpu::RequestAdapterOptions adapterOpts = {};
        adapterOpts.compatibleSurface = surface;
        wgpu::Adapter adapter = instance.requestAdapter(adapterOpts);
        std::cout << "Got adapter: " << adapter << std::endl;

        instance.release();

        std::cout << "Requesting device..." << std::endl;
        wgpu::DeviceDescriptor deviceDesc = {};
        deviceDesc.label = "My Device";
        deviceDesc.requiredFeatureCount = 0;
        deviceDesc.requiredLimits = nullptr;
        deviceDesc.defaultQueue.nextInChain = nullptr;
        deviceDesc.defaultQueue.label = "The default queue";
        // WARNING: Deprecated
        deviceDesc.deviceLostCallback = [](WGPUDeviceLostReason reason, char const *message, void * /* pUserData */)
        {
            std::cout << "Device lost: reason " << reason;
            if (message)
                std::cout << " (" << message << ")";
            std::cout << std::endl;
        };
        device = adapter.requestDevice(deviceDesc);
        std::cout << "Got device: " << device << std::endl;

        uncapturedErrorCallbackHandle = device.setUncapturedErrorCallback([](wgpu::ErrorType type, char const *message)
                                                                          {
		std::cout << "Uncaptured device error: type " << type;
		if (message) std::cout << " (" << message << ")";
		std::cout << std::endl; });

        queue = device.getQueue();

        // Configure the surface
        wgpu::SurfaceConfiguration config = {};

        // Configuration of the textures created for the underlying swap chain
        config.width = window.getWidth();
        config.height = window.getHeigth();
        config.usage = wgpu::TextureUsage::RenderAttachment;
        // WARNING: Deprecated
        surfaceFormat = surface.getPreferredFormat(adapter);
        config.format = surfaceFormat;

        // And we do not need any particular view format:
        config.viewFormatCount = 0;
        config.viewFormats = nullptr;
        config.device = device;
        config.presentMode = wgpu::PresentMode::Fifo;
        config.alphaMode = wgpu::CompositeAlphaMode::Auto;

        surface.configure(config);

        // Release the adapter only after it has been fully utilized
        adapter.release();

        initializePipeline();

        return true;
    }

    void Bear::draw()
    {
        // Get the next target texture view
        wgpu::TextureView targetView = getNextSurfaceTextureView();
        if (!targetView)
            return;

        // Create a command encoder for the draw call
        wgpu::CommandEncoderDescriptor encoderDesc = {};
        encoderDesc.label = "My command encoder";
        wgpu::CommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

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

        // Select which render pipeline to use
        renderPass.setPipeline(pipeline);
        // Draw 1 instance of a 3-vertices shape
        renderPass.draw(3, 1, 0, 0);

        renderPass.end();
        renderPass.release();

        // Finally encode and submit the render pass
        wgpu::CommandBufferDescriptor cmdBufferDescriptor = {};
        cmdBufferDescriptor.label = "Command buffer";
        wgpu::CommandBuffer command = encoder.finish(cmdBufferDescriptor);
        encoder.release();

        // std::cout << "Submitting command..." << std::endl;
        queue.submit(1, &command);
        command.release();
        // std::cout << "Command submitted." << std::endl;

        // WARNING: Deprecated, please use templated headers
        // auto onQueueWorkDone = [](WGPUQueueWorkDoneStatus status, void * /* pUserData */)
        // {
        //     std::cout << "Queued work finished with status: " << status << std::endl;
        // };
        // wgpuQueueOnSubmittedWorkDone(queue, onQueueWorkDone, nullptr /* pUserData */);

        // At the enc of the frame
        targetView.release();
#ifndef __EMSCRIPTEN__
        surface.present();
#endif

#if defined(WEBGPU_BACKEND_DAWN)
        device.tick();
#elif defined(WEBGPU_BACKEND_WGPU)
        device.poll(false);
#endif
    }

    void Bear::terminate()
    {
        pipeline.release();
        surface.unconfigure();
        queue.release();
        surface.release();
        device.release();
    }
    // We embbed the source of the shader module here
    const char *shaderSource = R"(
@vertex
fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f {
	var p = vec2f(0.0, 0.0);
	if (in_vertex_index == 0u) {
		p = vec2f(-0.5, -0.5);
	} else if (in_vertex_index == 1u) {
		p = vec2f(0.5, -0.5);
	} else {
		p = vec2f(0.0, 0.5);
	}
	return vec4f(p, 0.0, 1.0);
}

@fragment
fn fs_main() -> @location(0) vec4f {
	return vec4f(0.0, 0.4, 1.0, 1.0);
}
)";
    void Bear::initializePipeline()
    {

        // Load the shader module
        wgpu::ShaderModuleDescriptor shaderDesc;
#ifdef WEBGPU_BACKEND_WGPU
        shaderDesc.hintCount = 0;
        shaderDesc.hints = nullptr;
#endif

        // We use the extension mechanism to specify the WGSL part of the shader module descriptor
        wgpu::ShaderModuleWGSLDescriptor shaderCodeDesc;
        // Set the chained struct's header
        shaderCodeDesc.chain.next = nullptr;
        shaderCodeDesc.chain.sType = wgpu::SType::ShaderModuleWGSLDescriptor;
        // Connect the chain
        shaderDesc.nextInChain = &shaderCodeDesc.chain;
        shaderCodeDesc.code = shaderSource;
        wgpu::ShaderModule shaderModule = device.createShaderModule(shaderDesc);

        wgpu::RenderPipelineDescriptor pipelineDesc;
        // We do not use any vertex buffer for this first simplistic example
        pipelineDesc.vertex.bufferCount = 0;
        pipelineDesc.vertex.buffers = nullptr;

        // NB: We define the 'shaderModule' in the second part of this chapter.
        // Here we tell that the programmable vertex shader stage is described
        // by the function called 'vs_main' in that module.
        pipelineDesc.vertex.module = shaderModule;
        pipelineDesc.vertex.entryPoint = "vs_main";
        pipelineDesc.vertex.constantCount = 0;
        pipelineDesc.vertex.constants = nullptr;

        // Each sequence of 3 vertices is considered as a triangle
        pipelineDesc.primitive.topology = wgpu::PrimitiveTopology::TriangleList;

        // We'll see later how to specify the order in which vertices should be
        // connected. When not specified, vertices are considered sequentially.
        pipelineDesc.primitive.stripIndexFormat = wgpu::IndexFormat::Undefined;

        // The face orientation is defined by assuming that when looking
        // from the front of the face, its corner vertices are enumerated
        // in the counter-clockwise (CCW) order.
        pipelineDesc.primitive.frontFace = wgpu::FrontFace::CCW;

        // But the face orientation does not matter much because we do not
        // cull (i.e. "hide") the faces pointing away from us (which is often
        // used for optimization).
        pipelineDesc.primitive.cullMode = wgpu::CullMode::None;

        // We tell that the programmable fragment shader stage is described
        // by the function called 'fs_main' in the shader module.
        wgpu::FragmentState fragmentState;
        fragmentState.module = shaderModule;
        fragmentState.entryPoint = "fs_main";
        fragmentState.constantCount = 0;
        fragmentState.constants = nullptr;

        wgpu::BlendState blendState;
        blendState.color.srcFactor = wgpu::BlendFactor::SrcAlpha;
        blendState.color.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
        blendState.color.operation = wgpu::BlendOperation::Add;
        blendState.alpha.srcFactor = wgpu::BlendFactor::Zero;
        blendState.alpha.dstFactor = wgpu::BlendFactor::One;
        blendState.alpha.operation = wgpu::BlendOperation::Add;

        wgpu::ColorTargetState colorTarget;
        colorTarget.format = surfaceFormat;
        colorTarget.blend = &blendState;
        colorTarget.writeMask = wgpu::ColorWriteMask::All; // We could write to only some of the color channels.

        // We have only one target because our render pass has only one output color
        // attachment.
        fragmentState.targetCount = 1;
        fragmentState.targets = &colorTarget;
        pipelineDesc.fragment = &fragmentState;

        // We do not use stencil/depth testing for now
        pipelineDesc.depthStencil = nullptr;

        // Samples per pixel
        pipelineDesc.multisample.count = 1;

        // Default value for the mask, meaning "all bits on"
        pipelineDesc.multisample.mask = ~0u;

        // Default value as well (irrelevant for count = 1 anyways)
        pipelineDesc.multisample.alphaToCoverageEnabled = false;
        pipelineDesc.layout = nullptr;

        pipeline = device.createRenderPipeline(pipelineDesc);

        // We no longer need to access the shader module
        shaderModule.release();
    }

    wgpu::TextureView Bear::getNextSurfaceTextureView()
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
} // namespace gfx
