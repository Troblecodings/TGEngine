#pragma once

#include "..\stdbase.hpp"
#include "Device.hpp"
#include "Window\Window.hpp"
#include "Swapchain.hpp"
#include "CommandBuffer.hpp"
#include "Buffer\IndexBuffer.hpp"

SINCE(0, 0, 1)
void draw(VertexBuffer* vrt);

SINCE(0, 0, 1)
void createSemaphores();

SINCE(0, 0, 1)
void destroySemaphores();