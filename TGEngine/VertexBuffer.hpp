#pragma once

#include "TGEngine.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"
#include "Vertex.hpp"

extern VkBuffer vertex_buffer;
extern VkDeviceMemory device_memory;
extern uint32_t vertex_count;
extern VkMemoryRequirements requirements;

SINCE(0, 0, 1)
void createVertexBuffer(uint32_t max_vertex_count);

SINCE(0, 0, 2)
void fillVertexBuffer(std::vector<Vertex> vt);

SINCE(0, 0, 1)
void destroyVertexBuffer();