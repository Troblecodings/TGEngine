#pragma once

#include "../../public/Stdbase.hpp"

extern VkFramebuffer* frameBuffer;
extern VkImageView* imageView;

void createFramebuffer();

void destroyFrameBuffer();