#include "../../public/graphics/GUIModule.hpp"
#include "../../public/graphics/WindowModule.hpp"

#include "../../public/graphics/VulkanModuleDef.hpp"
#include <Windows.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_win32.cpp>
#include <backends/imgui_impl_win32.h>
#include <imgui.h>

namespace tge::gui {

using namespace vk;

inline void render(CommandBuffer buffer, RenderPass pass, void (*guicallback)()) {
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  guicallback();

  ImGui::Render();
  ImDrawData *draw_data = ImGui::GetDrawData();

  const CommandBufferInheritanceInfo inheritance(pass, 0);
  const CommandBufferBeginInfo beginInfo(
      CommandBufferUsageFlagBits::eSimultaneousUse |
          CommandBufferUsageFlagBits::eRenderPassContinue,
      &inheritance);
  buffer.begin(beginInfo);
  ImGui_ImplVulkan_RenderDrawData(draw_data, buffer);
  buffer.end();
}

main::Error GUIModule::init() {

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  winModule->customFn = (void*)ImGui_ImplWin32_WndProcHandler;
  const bool winInit = ImGui_ImplWin32_Init(winModule->hWnd);
  if (!winInit)
    return main::Error::COULD_NOT_CREATE_WINDOW;

  const auto vmod = (graphics::VulkanGraphicsModule *)this->api;

  VkDescriptorPoolSize pool_sizes[] = {
      {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
      {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
      {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
      {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
      {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
  VkDescriptorPoolCreateInfo pool_info = {};
  pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
  pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
  pool_info.pPoolSizes = pool_sizes;
  const auto result = vkCreateDescriptorPool(vmod->device, &pool_info, nullptr,
                                             ((VkDescriptorPool *)&pool));
  if (result != VK_SUCCESS)
    return main::Error::VULKAN_ERROR;

  ImGui_ImplVulkan_InitInfo instinfo = {vmod->instance,
                                        vmod->physicalDevice,
                                        vmod->device,
                                        vmod->queueFamilyIndex,
                                        vmod->queue,
                                        VK_NULL_HANDLE,
                                        (VkDescriptorPool)pool,
                                        0,
                                        3,
                                        3,
                                        VK_SAMPLE_COUNT_1_BIT,
                                        nullptr,
                                        [](VkResult rslt) {
                                          if (rslt != VK_SUCCESS)
                                            printf("ERROR IN VK");
                                        }};
  ImGui_ImplVulkan_Init(&instinfo, vmod->renderpass);

  const auto sCmd = vmod->cmdbuffer.back();
  const auto beginInfo =
      CommandBufferBeginInfo(CommandBufferUsageFlagBits::eOneTimeSubmit);
  sCmd.begin(beginInfo);
  ImGui_ImplVulkan_CreateFontsTexture(sCmd);
  sCmd.end();

  const auto submitInfo = SubmitInfo({}, {}, sCmd, {});
  vmod->queue.submit(submitInfo);

  vmod->device.waitIdle();
  ImGui_ImplVulkan_DestroyFontUploadObjects();

  const auto allocInfo =
      CommandBufferAllocateInfo(vmod->pool, CommandBufferLevel::eSecondary, 1);
  buffer = vmod->device.allocateCommandBuffers(allocInfo).back();
  vmod->secondaryCommandBuffer.push_back((VkCommandBuffer)buffer);

  render((VkCommandBuffer)buffer, vmod->renderpass, guicallback);

  return main::Error::NONE;
}

void GUIModule::tick(double deltatime) {
  const auto vmod = (graphics::VulkanGraphicsModule *)this->api;
  render((VkCommandBuffer)this->buffer, vmod->renderpass, guicallback);
}

void GUIModule::destroy() {
  const auto vmod = (graphics::VulkanGraphicsModule *)this->api;
  vmod->device.waitIdle();
  ImGui_ImplVulkan_Shutdown();
  vmod->device.destroyDescriptorPool(((VkDescriptorPool)pool));
  ImGui_ImplWin32_Shutdown();
}

} // namespace tge::gui
