#pragma once

#include "../Stdbase.hpp"

extern VkFramebuffer* framenBuffer;
extern VkImageView* imageView;

void createFramebuffer();

void destroyFrameBuffer();