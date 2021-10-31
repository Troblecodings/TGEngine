#include "../../public/graphics/GUIModule.hpp"
#include "../../public/graphics/WindowModule.hpp"

#include "../../public/graphics/vulkan/VulkanModuleDef.hpp"
#include <Windows.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_win32.cpp>
#include <backends/imgui_impl_win32.h>
#include <imgui.h>
#include "../../public/TGEngine.hpp"

namespace tge::gui {

using namespace vk;

inline void render(gui::GUIModule* gmod) {
  const CommandBuffer buffer = (VkCommandBuffer)gmod->buffer;
  const RenderPass pass = (VkRenderPass)gmod->renderpass;
  auto vgm = (graphics::VulkanGraphicsModule *)main::getAPILayer();
  const Framebuffer frame = ((Framebuffer *)gmod->framebuffer)[vgm->nextImage];

  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  gmod->renderGUI();

  ImGui::Render();
  ImDrawData *draw_data = ImGui::GetDrawData();

  const CommandBufferBeginInfo beginInfo;
  buffer.begin(beginInfo);

  constexpr std::array clearColor = {1.0f, 1.0f, 1.0f, 1.0f};

  const std::array clearValue = {ClearValue(clearColor),
                                 ClearValue(ClearDepthStencilValue(0.0f, 0))};

  const RenderPassBeginInfo renderPassBeginInfo(
      pass, frame,
      {{0, 0}, {(uint32_t)vgm->viewport.width, (uint32_t)vgm->viewport.height}},
      clearValue);
  buffer.beginRenderPass(renderPassBeginInfo, {});
  ImGui_ImplVulkan_RenderDrawData(draw_data, buffer);
  buffer.endRenderPass();
  buffer.end();
}

main::Error GUIModule::init() {

  auto winModule = main::getGameGraphicsModule()->getWindowModule();
  auto api = main::getAPILayer();
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  winModule->customFn = (void *)ImGui_ImplWin32_WndProcHandler;
  const bool winInit = ImGui_ImplWin32_Init(winModule->hWnd);
  if (!winInit)
    return main::Error::COULD_NOT_CREATE_WINDOW;

  const auto vmod = (graphics::VulkanGraphicsModule *)api;

  const std::array attachments = {
      AttachmentDescription(
          {}, vmod->format.format, SampleCountFlagBits::e1,
          AttachmentLoadOp::eLoad, AttachmentStoreOp::eStore,
          AttachmentLoadOp::eDontCare, AttachmentStoreOp::eDontCare,
          ImageLayout::eUndefined, ImageLayout::ePresentSrcKHR)};

  constexpr std::array colorAttachments = {
      AttachmentReference(0, ImageLayout::eColorAttachmentOptimal)};

  const std::array subpassDescriptions = {
      SubpassDescription({}, PipelineBindPoint::eGraphics, {}, colorAttachments)};

  const std::array subpassDependencies = {
      SubpassDependency(VK_SUBPASS_EXTERNAL, 0,
                        PipelineStageFlagBits::eColorAttachmentOutput |
                            PipelineStageFlagBits::eEarlyFragmentTests,
                        PipelineStageFlagBits::eColorAttachmentOutput |
                            PipelineStageFlagBits::eEarlyFragmentTests,
                        (AccessFlagBits)0,
                        AccessFlagBits::eColorAttachmentWrite |
                            AccessFlagBits::eColorAttachmentRead |
                            AccessFlagBits::eDepthStencilAttachmentRead |
                            AccessFlagBits::eDepthStencilAttachmentWrite)};

  const RenderPassCreateInfo renderPassCreateInfo(
      {}, attachments, subpassDescriptions, subpassDependencies);
  this->renderpass = vmod->device.createRenderPass(renderPassCreateInfo);

  framebuffer = new Framebuffer[vmod->swapchainImageviews.size()];

  for (size_t i = 0; i < vmod->swapchainImageviews.size(); i++) {
    const auto imview = vmod->swapchainImageviews[i];
    const FramebufferCreateInfo framebufferCreateInfo(
        {}, (VkRenderPass)renderpass, imview, vmod->viewport.width,
        vmod->viewport.height, 1);
    ((Framebuffer *)framebuffer)[i] =
        vmod->device.createFramebuffer(framebufferCreateInfo);
  }

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
  ImGui_ImplVulkan_Init(&instinfo, (VkRenderPass)this->renderpass);

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
      CommandBufferAllocateInfo(vmod->pool, CommandBufferLevel::ePrimary, 1);
  buffer = vmod->device.allocateCommandBuffers(allocInfo).back();

  render(this);

  vmod->primary.push_back((VkCommandBuffer)buffer);

  return main::Error::NONE;
}

void GUIModule::tick(double deltatime) {
  render(this);
}

void GUIModule::destroy() {
  const auto vmod = (graphics::VulkanGraphicsModule *)main::getAPILayer();
  vmod->device.waitIdle();
  ImGui_ImplVulkan_Shutdown();
  vmod->device.destroyDescriptorPool(((VkDescriptorPool)pool));
  vmod->device.destroyRenderPass((VkRenderPass)this->renderpass);
  for (size_t i = 0; i < vmod->swapchainImageviews.size(); i++) {
    vmod->device.destroyFramebuffer(((Framebuffer *)framebuffer)[i]);
  }
  delete[](Framebuffer *) framebuffer;
  ImGui_ImplWin32_Shutdown();
}

} // namespace tge::gui
