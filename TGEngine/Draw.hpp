#pragma once

#include "TGEngine.hpp"
#include "Device.hpp"
#include "Window.hpp"
#include "Swapchain.hpp"
#include "CommandBuffer.hpp"
#include "IndexBuffer.hpp"

SINCE(0, 0, 1)
void draw(std::vector<Vertex>* vrt);

SINCE(0, 0, 1)
void createSemaphores();

SINCE(0, 0, 1)
void destroySemaphores();