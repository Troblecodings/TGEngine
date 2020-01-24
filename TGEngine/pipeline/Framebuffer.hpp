#pragma once

#include "../Stdbase.hpp"

extern VkFramebuffer* frameBuffer;
extern VkImageView* imageView;

void createFramebuffer();

void destroyFrameBuffer();