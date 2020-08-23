﻿cmake_minimum_required (VERSION 3.8)

project ("TGTools")
set (CMAKE_CXX_STANDARD 20)

include_directories("../dependencies/Include" "../dependencies/Third-Party/Include")

add_library (TGEngine STATIC 
	"public/Error.hpp"
	"public/Stdbase.hpp"
	"public/TGEngine.hpp"
	"public/drawlib/Quad.hpp"
	"public/gamecontent/Actor.hpp"
	"public/gamecontent/Light.hpp"
	"public/gamecontent/Material.hpp"
	"public/gamecontent/PlayerController.hpp"
	"public/gamecontent/camera/2DCamera.hpp"
	"public/gamecontent/camera/Camera.hpp"
	"public/gamecontent/camera/FirstPersonCamera.hpp"
	"public/gamecontent/camera/StaticCamera.hpp"
	"public/io/Files.hpp"
	"public/io/Font.hpp"
	"public/io/Keyboard.hpp"
	"public/io/Mouse.hpp"
	"public/io/Properties.hpp"
	"public/io/Resource.hpp"
	"public/pipeline/CommandBuffer.hpp"
	"public/pipeline/DepthStencil.hpp"
	"public/pipeline/Descriptors.hpp"
	"public/pipeline/Device.hpp"
	"public/pipeline/Draw.hpp"
	"public/pipeline/Framebuffer.hpp"
	"public/pipeline/Instance.hpp"
	"public/pipeline/Pipe.hpp"
	"public/pipeline/Pipeline.hpp"
	"public/pipeline/PrePipeline.hpp"
	"public/pipeline/RenderPass.hpp"
	"public/pipeline/ShaderCreation.hpp"
	"public/pipeline/Swapchain.hpp"
	"public/pipeline/buffer/Memory.hpp"
	"public/pipeline/buffer/StagingBuffer.hpp"
	"public/pipeline/buffer/Texturebuffer.hpp"
	"public/pipeline/buffer/UniformBuffer.hpp"
	"public/pipeline/window/Window.hpp"
	"public/ui/UISystem.hpp"
	"public/util/Debug.hpp"
	"public/util/Math.hpp"
	
	"public/util/VectorUtil.hpp"
	"private/Stdbase.cpp"
	"private/TGEngine.cpp"
	"private/drawlib/Quad.cpp"
	"private/gamecontent/Actor.cpp"
	"private/gamecontent/Light.cpp"
	"private/gamecontent/Material.cpp"
	"private/gamecontent/PlayerController.cpp"
	"private/gamecontent/camera/2DCamera.cpp"
	"private/gamecontent/camera/Camera.cpp"
	"private/gamecontent/camera/FirstPersonCamera.cpp"
	"private/gamecontent/camera/StaticCamera.cpp"
	"private/io/Files.cpp"
	"private/io/Font.cpp"
	"private/io/Keyboard.cpp"
	"private/io/Mouse.cpp"
	"private/io/Properties.cpp"
	"private/io/Resource.cpp"
	"private/pipeline/CommandBuffer.cpp"
	"private/pipeline/DepthStencil.cpp"
	"private/pipeline/Descriptors.cpp"
	"private/pipeline/Device.cpp"
	"private/pipeline/Draw.cpp"
	"private/pipeline/Framebuffer.cpp"
	"private/pipeline/Instance.cpp"
	"private/pipeline/Pipe.cpp"
	"private/pipeline/PrePipeline.cpp"
	"private/pipeline/RenderPass.cpp"
	"private/pipeline/ShaderCreation.cpp"
	"private/pipeline/Swapchain.cpp"
	"private/pipeline/buffer/Memory.cpp"
	"private/pipeline/buffer/StagingBuffer.cpp"
	"private/pipeline/buffer/Texturebuffer.cpp"
	"private/pipeline/buffer/UniformBuffer.cpp"
	"private/pipeline/window/Window.cpp"
	"private/resources/ShaderData.cpp"
	"public/resources/ShaderData.hpp"
	"private/resources/ShaderPipes.cpp"
	"public/resources/ShaderPipes.hpp"
	"private/ui/UISystem.cpp"
	"private/util/Debug.cpp"
	
	"private/util/VectorUtil.cpp"
)