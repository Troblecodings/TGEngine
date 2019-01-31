#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "window/Window.hpp"
#include "Swapchain.hpp"
#include "CommandBuffer.hpp"
#include "buffer/IndexBuffer.hpp"

extern uint32_t image_index;
extern VkFence fence;

SINCE(0, 0, 3)
void startdraw();

SINCE(0, 0, 3)
void present();

SINCE(0, 0, 1)
void submit();

SINCE(0, 0, 1)
void createSemaphores();

SINCE(0, 0, 1)
void destroySemaphores();