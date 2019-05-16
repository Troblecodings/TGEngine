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
vlib_image_memory_barrier.oldLayout = oldlay;\
vlib_image_memory_barrier.newLayout = newlay;\
vlib_image_memory_barrier.image = img;\
vlib_image_memory_barrier.srcAccessMask = srcm;\
vlib_image_memory_barrier.dstAccessMask = dstm;\
vkCmdPipelineBarrier(buf, srcst, dstst, 0, 0, nullptr, 0, nullptr, 1, &vlib_image_memory_barrier);

extern VkBufferImageCopy vlib_buffer_image_copy;
extern VkImageCreateInfo vlib_image_create_info;
extern VkImageViewCreateInfo vlib_image_view_create_info;
extern VkImageMemoryBarrier vlib_image_memory_barrier;
extern VkImageBlit vlib_image_blit;
extern VkSamplerCreateInfo vlibSamplerCreateInfo;