#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "window/Window.hpp"
#include "Swapchain.hpp"
#include "CommandBuffer.hpp"
#include "buffer/IndexBuffer.hpp"

extern uint32_t image_index;
extern VkFence fence;

void startdraw(IndexBuffer* ibuffer, VertexBuffer* vbuffer);

void present(IndexBuffer* ibuffer, VertexBuffer* vbuffer);

void submit(IndexBuffer* ibuffer, VertexBuffer* vbuffer);

void createSemaphores();

void destroySemaphores();