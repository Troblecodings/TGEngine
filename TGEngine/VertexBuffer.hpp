#pragma once

#include "TGEngine.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"
#include "Vertex.hpp"

extern VkBuffer vertex_buffer;
extern VkDeviceMemory device_memory;
extern uint32_t vertex_count;

SINCE(0, 0, 1)
void createVertexBuffer(std::vector<Vertex> vt);

SINCE(0, 0, 1)
void destroyVertexBuffer();