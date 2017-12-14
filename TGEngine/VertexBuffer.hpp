#pragma once

#include "TGEngine.hpp"
#include "Device.hpp"
#include "Vertex.hpp"
#include "Memory.hpp"

extern VkBuffer vertex_buffer;
extern uint32_t vertex_buffer_index;;
extern uint32_t vertex_count;

SINCE(0, 0, 1)
void createVertexBuffer(uint32_t max_vertex_count);

SINCE(0, 0, 2)
void fillVertexBuffer(std::vector<Vertex>* vt);