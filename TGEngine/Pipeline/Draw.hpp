#pragma once

#include "..\stdbase.hpp"
#include "Device.hpp"
#include "Window\Window.hpp"
#include "Swapchain.hpp"
#include "CommandBuffer.hpp"
#include "Buffer\IndexBuffer.hpp"


SINCE(0, 0, 3)
void startdraw();

SINCE(0, 0, 3)
void present();

SINCE(0, 0, 1)
void draw();

SINCE(0, 0, 1)
void createSemaphores();

SINCE(0, 0, 1)
void destroySemaphores();