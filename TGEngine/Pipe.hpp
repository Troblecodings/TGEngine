#pragma once

#include "TGEngine.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include <vector>
#include "Window.hpp"

extern VkPipeline pipeline;
extern VkPipelineLayout layout;
extern VkViewport viewport;
extern VkRect2D scissor;

SINCE(0, 0, 1)
void createPipeline();

SINCE(0, 0, 1)
void destroyPipeline();