#pragma once

#include "../Stdbase.hpp"

/*
 * Parameters:
 *   buf -> the given command buffer to add the barrier to
 *   oldlay -> the old layout of the image
 *   newlay -> the layout until it blocks
 *   img -> the image for this barrier
 *   srcm -> the VkAccessFlags of the source
 *   dstm -> the VkAccessFlags of the destination
 *   srcst -> the VkPipelineStageFlags of the source
 *   dstst -> the VkPipelineStageFlags of the destination
 */
#define ADD_IMAGE_MEMORY_BARRIER(buf, oldlay, newlay, img, srcm, dstm, srcst, dstst) \
vlibImageMemoryBarrier.oldLayout = oldlay;\
vlibImageMemoryBarrier.newLayout = newlay;\
vlibImageMemoryBarrier.image = img;\
vlibImageMemoryBarrier.srcAccessMask = srcm;\
vlibImageMemoryBarrier.dstAccessMask = dstm;\
vkCmdPipelineBarrier(buf, srcst, dstst, 0, 0, nullptr, 0, nullptr, 1, &vlibImageMemoryBarrier);

extern VkBufferImageCopy vlibBufferImageCopy;
extern VkImageCreateInfo vlibImageCreateInfo;
extern VkImageViewCreateInfo vlibImageViewCreateInfo;
extern VkImageMemoryBarrier vlibImageMemoryBarrier;
extern VkImageBlit vlibImageBlit;
extern VkSamplerCreateInfo vlibSamplerCreateInfo;