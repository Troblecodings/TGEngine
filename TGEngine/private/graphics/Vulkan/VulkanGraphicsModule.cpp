#include "../../../public/graphics/vulkan/VulkanGraphicsModule.hpp"
#include "../../../public/Error.hpp"
#include "../../../public/Util.hpp"
#include "../../../public/graphics/WindowModule.hpp"
#include <array>
#include <iostream>
#include <mutex>
#define VULKAN_HPP_HAS_SPACESHIP_OPERATOR
#include "../../../public/graphics/vulkan/VulkanModuleDef.hpp"
#include <unordered_set>

namespace tge::graphics
{

  using namespace tge::shader;

  constexpr std::array layerToEnable = {"VK_LAYER_KHRONOS_validation",
                                        "VK_LAYER_VALVE_steam_overlay",
                                        "VK_LAYER_NV_optimus"};

  constexpr std::array extensionToEnable = {VK_KHR_SURFACE_EXTENSION_NAME
#ifdef WIN32
                                            ,
                                            VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif // WIN32
#ifdef DEBUG
                                            ,
                                            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif

  };

  using namespace vk;

  Result verror = Result::eSuccess;

#define VERROR(rslt)                                               \
  if (rslt != Result::eSuccess)                                    \
  {                                                                \
    verror = rslt;                                                 \
    main::error = main::Error::VULKAN_ERROR;                       \
    std::string s = to_string(verror);                             \
    const auto file = __FILE__;                                    \
    const auto line = __LINE__;                                    \
    printf("Vulkan error %s in %s L%d!\n", s.c_str(), file, line); \
  } // namespace tge::graphics

  inline void waitForImageTransition(
      const CommandBuffer &curBuffer, const ImageLayout oldLayout,
      const ImageLayout newLayout, const Image image,
      const ImageSubresourceRange &subresource,
      const PipelineStageFlags srcFlags = PipelineStageFlagBits::eTopOfPipe,
      const AccessFlags srcAccess = AccessFlagBits::eNoneKHR,
      const PipelineStageFlags dstFlags = PipelineStageFlagBits::eAllGraphics,
      const AccessFlags dstAccess = AccessFlagBits::eNoneKHR)
  {
    const ImageMemoryBarrier imageMemoryBarrier(
        srcAccess, dstAccess, oldLayout, newLayout, VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED, image, subresource);
    curBuffer.pipelineBarrier(srcFlags, dstFlags, DependencyFlagBits::eByRegion,
                              {}, {}, imageMemoryBarrier);
  }

  constexpr PipelineInputAssemblyStateCreateInfo
      inputAssemblyCreateInfo({}, PrimitiveTopology::eTriangleList,
                              false); // For now constexpr

#define EXPECT(assertion)                                \
  if (!this->isInitialiazed || !(assertion))             \
  {                                                      \
    throw std::runtime_error("Debug assertion failed!"); \
  }

  void *VulkanGraphicsModule::loadShader(const MaterialType type)
  {
    EXPECT(((size_t)type) <= (size_t)MAX_TYPE);
    const auto idx = (size_t)type;
    auto &vert = shaderNames[idx];
    const auto ptr = shaderAPI->loadShaderPipeAndCompile(vert);
    return ptr;
  }

  size_t VulkanGraphicsModule::pushMaterials(const size_t materialcount,
                                             const Material *materials)
  {
    EXPECT(materialcount != 0 && materials != nullptr);

    const Rect2D scissor({0, 0},
                         {(uint32_t)viewport.width, (uint32_t)viewport.height});
    const PipelineViewportStateCreateInfo pipelineViewportCreateInfo({}, viewport,
                                                                     scissor);

    const PipelineMultisampleStateCreateInfo multisampleCreateInfo(
        {}, SampleCountFlagBits::e1, false, 1);

    const auto pDefaultState = PipelineColorBlendAttachmentState(
        true, BlendFactor::eSrcAlpha, BlendFactor::eOneMinusSrcAlpha,
        BlendOp::eAdd, BlendFactor::eOne, BlendFactor::eZero, BlendOp::eAdd,
        (ColorComponentFlags)FlagTraits<ColorComponentFlagBits>::allFlags);

    const std::array blendAttachment = {pDefaultState, pDefaultState,
                                        pDefaultState, pDefaultState};

    const PipelineColorBlendStateCreateInfo colorBlendState(
        {}, false, LogicOp::eClear, blendAttachment);

    const PipelineDepthStencilStateCreateInfo pipeDepthState(
        {}, true, true, CompareOp::eLess, false, false, {}, {}, 0, 1);

    std::vector<GraphicsPipelineCreateInfo> pipelineCreateInfos;
    pipelineCreateInfos.reserve(materialcount);

    for (size_t i = 0; i < materialcount; i++)
    {
      const auto &material = materials[i];

      const auto shaderPipe = (VulkanShaderPipe *)material.costumShaderData;

      shaderPipe->pipelineShaderStage.clear();
      shaderPipe->pipelineShaderStage.reserve(shaderPipe->shader.size());

      for (const auto &shaderPair : shaderPipe->shader)
      {
        const auto &shaderData = shaderPair.first;

        const ShaderModuleCreateInfo shaderModuleCreateInfo(
            {}, shaderData.size() * sizeof(uint32_t), shaderData.data());
        const auto shaderModule =
            device.createShaderModule(shaderModuleCreateInfo);
        shaderModules.push_back(shaderModule);
        shaderPipe->pipelineShaderStage.push_back(PipelineShaderStageCreateInfo(
            {}, shaderPair.second, shaderModule, "main"));
      }

      shaderPipe->rasterization.frontFace = FrontFace::eCounterClockwise;
      shaderPipe->rasterization.lineWidth = 1;
      shaderPipe->rasterization.depthBiasEnable = false;
      shaderPipe->rasterization.depthClampEnable = false;
      shaderPipe->rasterization.rasterizerDiscardEnable = false;
      shaderPipe->rasterization.cullMode = material.doubleSided
                                               ? CullModeFlagBits::eNone
                                               : CullModeFlagBits::eFront;

      GraphicsPipelineCreateInfo gpipeCreateInfo(
          {}, shaderPipe->pipelineShaderStage, &shaderPipe->inputStateCreateInfo,
          &inputAssemblyCreateInfo, {}, &pipelineViewportCreateInfo,
          &shaderPipe->rasterization, &multisampleCreateInfo, &pipeDepthState,
          &colorBlendState, {}, {}, renderpass, 0);
      shaderAPI->addToMaterial(&material, &gpipeCreateInfo);
      pipelineCreateInfos.push_back(gpipeCreateInfo);
      shaderPipes.push_back(shaderPipe);
    }

    const auto piperesult =
        device.createGraphicsPipelines({}, pipelineCreateInfos);
    VERROR(piperesult.result);
    const auto indexOffset = pipelines.size();
    pipelines.resize(indexOffset + piperesult.value.size());
    std::copy(piperesult.value.cbegin(), piperesult.value.cend(),
              pipelines.begin() + indexOffset);
    return indexOffset;
  }

  void VulkanGraphicsModule::pushRender(const size_t renderInfoCount,
                                        const RenderInfo *renderInfos)
  {
    EXPECT(renderInfoCount != 0 && renderInfos != nullptr);

    const CommandBufferAllocateInfo commandBufferAllocate(
        pool, CommandBufferLevel::eSecondary, 1);
    const CommandBuffer cmdBuf =
        device.allocateCommandBuffers(commandBufferAllocate).back();

    const CommandBufferInheritanceInfo inheritance(renderpass, 0);
    const CommandBufferBeginInfo beginInfo(
        CommandBufferUsageFlagBits::eRenderPassContinue, &inheritance);
    cmdBuf.begin(beginInfo);
    for (size_t i = 0; i < renderInfoCount; i++)
    {
      auto &info = renderInfos[i];

      std::vector<Buffer> vertexBuffer;
      vertexBuffer.reserve(info.vertexBuffer.size());

      for (auto vertId : info.vertexBuffer)
      {
        vertexBuffer.push_back(bufferList[vertId]);
      }

      if (info.vertexOffsets.size() == 0)
      {
        std::vector<size_t> offsets(vertexBuffer.size());
        std::fill(offsets.begin(), offsets.end(), 0);
        cmdBuf.bindVertexBuffers(0, vertexBuffer, offsets);
      }
      else
      {
        cmdBuf.bindVertexBuffers(0, vertexBuffer, info.vertexOffsets);
      }

      if (info.bindingID != UINT64_MAX)
      {
        shaderAPI->addToRender(&info.bindingID, 1, (void *)&cmdBuf);
      }
      else
      {
        const auto binding =
            shaderAPI->createBindings(shaderPipes[info.materialId]);
        shaderAPI->addToRender(&binding, 1, (void *)&cmdBuf);
      }

      cmdBuf.bindPipeline(PipelineBindPoint::eGraphics,
                          pipelines[info.materialId]);

      if (info.indexSize != IndexSize::NONE) [[likely]]
      {
        cmdBuf.bindIndexBuffer(bufferList[info.indexBuffer], info.indexOffset,
                               (IndexType)info.indexSize);

        cmdBuf.drawIndexed(info.indexCount, info.instanceCount, 0, 0, 0);
      }
      else
      {
        cmdBuf.draw(info.indexCount, info.instanceCount, 0, 0);
      }
    }
    cmdBuf.end();
    const std::lock_guard onExitUnlock(commandBufferRecording);
    secondaryCommandBuffer.push_back(cmdBuf);
  }

  inline void submitAndWait(const Device &device, const Queue &queue,
                            const CommandBuffer &cmdBuf)
  {
    const FenceCreateInfo fenceCreateInfo;
    const auto fence = device.createFence(fenceCreateInfo);

    const SubmitInfo submitInfo({}, {}, cmdBuf, {});
    queue.submit(submitInfo, fence);

    const Result result = device.waitForFences(fence, true, UINT64_MAX);
    VERROR(result);

    device.destroyFence(fence);
  }

  inline BufferUsageFlags getUsageFlagsFromDataType(const DataType type)
  {
    switch (type)
    {
    case DataType::VertexIndexData:
      return BufferUsageFlagBits::eVertexBuffer |
             BufferUsageFlagBits::eIndexBuffer;
    case DataType::Uniform:
      return BufferUsageFlagBits::eUniformBuffer;
    case DataType::VertexData:
      return BufferUsageFlagBits::eVertexBuffer;
    case DataType::IndexData:
      return BufferUsageFlagBits::eIndexBuffer;
    default:
      throw std::runtime_error("Couldn't find usage flag");
    }
  }

  size_t VulkanGraphicsModule::pushData(const size_t dataCount,
                                        const uint8_t **data,
                                        const size_t *dataSizes,
                                        const DataType type)
  {
    EXPECT(dataCount != 0 && data != nullptr && dataSizes != nullptr);

    std::vector<DeviceMemory> tempMemory;
    tempMemory.reserve(dataCount);
    std::vector<Buffer> tempBuffer;
    tempBuffer.reserve(dataCount);

    const auto firstIndex = bufferList.size();
    bufferList.reserve(firstIndex + dataCount);
    const auto firstMemIndex = bufferMemoryList.size();
    bufferMemoryList.reserve(firstMemIndex + dataCount);

    const auto cmdBuf = cmdbuffer.back();

    const CommandBufferBeginInfo beginInfo(
        CommandBufferUsageFlagBits::eOneTimeSubmit);
    cmdBuf.begin(beginInfo);

    const BufferUsageFlags bufferUsage = getUsageFlagsFromDataType(type);

    for (size_t i = 0; i < dataCount; i++)
    {
      const auto size = dataSizes[i];
      const auto dataptr = data[i];

      const BufferCreateInfo bufferCreateInfo(
          {}, size, BufferUsageFlagBits::eTransferSrc, SharingMode::eExclusive);
      const auto intermBuffer = device.createBuffer(bufferCreateInfo);
      tempBuffer.push_back(intermBuffer);
      const auto memRequ = device.getBufferMemoryRequirements(intermBuffer);

      const MemoryAllocateInfo allocInfo(memRequ.size,
                                         memoryTypeHostVisibleCoherent);
      const auto hostVisibleMemory = device.allocateMemory(allocInfo);
      tempMemory.push_back(hostVisibleMemory);
      device.bindBufferMemory(intermBuffer, hostVisibleMemory, 0);
      const auto mappedHandle =
          device.mapMemory(hostVisibleMemory, 0, VK_WHOLE_SIZE);
      memcpy(mappedHandle, dataptr, size);
      device.unmapMemory(hostVisibleMemory);

      const BufferCreateInfo bufferLocalCreateInfo(
          {}, size,
          BufferUsageFlagBits::eTransferDst | BufferUsageFlagBits::eTransferSrc |
              bufferUsage,
          SharingMode::eExclusive);
      const auto localBuffer = device.createBuffer(bufferLocalCreateInfo);
      bufferList.push_back(localBuffer);
      const auto memRequLocal = device.getBufferMemoryRequirements(localBuffer);
      const MemoryAllocateInfo allocLocalInfo(memRequLocal.size,
                                              memoryTypeDeviceLocal);
      const auto localMem = device.allocateMemory(allocLocalInfo);
      device.bindBufferMemory(localBuffer, localMem, 0);
      bufferMemoryList.push_back(localMem);
      bufferSizeList.push_back(size);

      const BufferCopy copyInfo(0, 0, size);
      cmdBuf.copyBuffer(intermBuffer, localBuffer, copyInfo);
    }

    cmdBuf.end();

    submitAndWait(device, queue, cmdBuf);

    for (const auto mem : tempMemory)
      device.freeMemory(mem);
    for (const auto buf : tempBuffer)
      device.destroyBuffer(buf);

    return firstIndex;
  }

  void VulkanGraphicsModule::changeData(const size_t bufferIndex,
                                        const uint8_t *data,
                                        const size_t dataSizes,
                                        const size_t offset)
  {
    EXPECT(bufferIndex >= 0 && bufferIndex < this->bufferList.size() &&
           data != nullptr && dataSizes != 0);

    const BufferCreateInfo bufferCreateInfo({}, dataSizes,
                                            BufferUsageFlagBits::eTransferSrc,
                                            SharingMode::eExclusive);
    const auto intermBuffer = device.createBuffer(bufferCreateInfo);
    const auto memRequ = device.getBufferMemoryRequirements(intermBuffer);

    const MemoryAllocateInfo allocInfo(memRequ.size,
                                       memoryTypeHostVisibleCoherent);
    const auto hostVisibleMemory = device.allocateMemory(allocInfo);
    device.bindBufferMemory(intermBuffer, hostVisibleMemory, 0);
    const auto mappedHandle =
        device.mapMemory(hostVisibleMemory, 0, VK_WHOLE_SIZE);

    memcpy(mappedHandle, data, dataSizes);

    device.unmapMemory(hostVisibleMemory);

    const auto cmdBuf = cmdbuffer.back();

    const CommandBufferBeginInfo beginInfo(
        CommandBufferUsageFlagBits::eOneTimeSubmit);
    cmdBuf.begin(beginInfo);

    const BufferCopy copyRegion(0, offset, dataSizes);

    cmdBuf.copyBuffer(intermBuffer, this->bufferList[bufferIndex], copyRegion);

    cmdBuf.end();

    submitAndWait(device, queue, cmdBuf);
    device.freeMemory(hostVisibleMemory);
    device.destroyBuffer(intermBuffer);
  }

  size_t VulkanGraphicsModule::pushSampler(const SamplerInfo &sampler)
  {
    const auto position = this->sampler.size();
    const SamplerCreateInfo samplerCreateInfo(
        {}, (Filter)sampler.minFilter, (Filter)sampler.magFilter,
        SamplerMipmapMode::eLinear, (SamplerAddressMode)sampler.uMode,
        (SamplerAddressMode)sampler.vMode, (SamplerAddressMode)sampler.vMode, 0,
        sampler.anisotropy, sampler.anisotropy);
    const auto smplr = device.createSampler(samplerCreateInfo);
    this->sampler.push_back(smplr);
    return position;
  }

  struct InternalImageInfo
  {
    Format format;
    Extent2D ex;
    ImageUsageFlags usage = ImageUsageFlagBits::eColorAttachment;
    SampleCountFlagBits sampleCount = SampleCountFlagBits::e1;
  };

  inline size_t
  createInternalImages(VulkanGraphicsModule *vgm,
                       const std::vector<InternalImageInfo> &imagesIn)
  {
    std::vector<std::tuple<ImageViewCreateInfo, size_t>> memorys;
    size_t wholeSize = 0;

    const auto firstIndex = vgm->textureImages.size();

    for (const auto &img : imagesIn)
    {
      const ImageCreateInfo depthImageCreateInfo(
          {}, ImageType::e2D, img.format, {img.ex.width, img.ex.height, 1}, 1, 1,
          img.sampleCount, ImageTiling::eOptimal, img.usage);
      const auto depthImage = vgm->device.createImage(depthImageCreateInfo);

      const MemoryRequirements imageMemReq =
          vgm->device.getImageMemoryRequirements(depthImage);

      const ImageAspectFlags aspect =
          ((img.usage & ImageUsageFlagBits::eColorAttachment ||
            img.usage & ImageUsageFlagBits::eSampled)
               ? ImageAspectFlagBits::eColor
               : (ImageAspectFlagBits)0) |
          (img.usage & ImageUsageFlagBits::eDepthStencilAttachment
               ? ImageAspectFlagBits::eDepth
               : (ImageAspectFlagBits)0);
      const ImageSubresourceRange subresourceRange(aspect, 0, 1, 0, 1);

      const ImageViewCreateInfo depthImageViewCreateInfo(
          {}, depthImage, ImageViewType::e2D, img.format, {}, subresourceRange);
      vgm->textureImages.push_back(depthImage);

      const auto rest = wholeSize % imageMemReq.alignment;
      if (rest != 0) {
        wholeSize += imageMemReq.alignment - rest;
      }

      memorys.push_back(std::make_tuple(depthImageViewCreateInfo, wholeSize));
      wholeSize += imageMemReq.size;
    }

    const MemoryAllocateInfo memAllocInfo(wholeSize, vgm->memoryTypeDeviceLocal);
    const auto depthImageMemory = vgm->device.allocateMemory(memAllocInfo);

    for (const auto &[image, offset] : memorys)
    {
      vgm->device.bindImageMemory(image.image, depthImageMemory, offset);
      vgm->textureMemorys.push_back(std::make_tuple(depthImageMemory, offset));

      const auto depthImageView = vgm->device.createImageView(image);
      vgm->textureImageViews.push_back(depthImageView);
    }

    return firstIndex;
  }

  size_t VulkanGraphicsModule::pushTexture(const size_t textureCount,
                                           const TextureInfo *textures)
  {
    EXPECT(textureCount != 0 && textures != nullptr);

    const size_t firstIndex = textureImages.size();

    std::vector<Buffer> intermBuffers;
    std::vector<DeviceMemory> intermMemorys;
    std::vector<BufferImageCopy> intermCopys;
    intermBuffers.reserve(textureCount);
    intermMemorys.reserve(textureCount);
    intermCopys.reserve(textureCount);

    util::OnExit exitHandle([&]
                            {
                              for (auto mem : intermMemorys)
                                device.freeMemory(mem);
                              for (auto img : intermBuffers)
                                device.destroyBuffer(img);
                            });

    textureImages.reserve(firstIndex + textureCount);
    textureMemorys.reserve(firstIndex + textureCount);
    textureImageViews.reserve(firstIndex + textureCount);

    const auto cmd = this->cmdbuffer.back();

    const CommandBufferBeginInfo beginInfo(
        CommandBufferUsageFlagBits::eOneTimeSubmit, {});
    cmd.begin(beginInfo);

    constexpr ImageSubresourceRange range = {ImageAspectFlagBits::eColor, 0, 1, 0,
                                             1};
    const Format format = Format::eR8G8B8A8Unorm;

    std::vector<InternalImageInfo> imagesIn(textureCount);
    for (size_t i = 0; i < textureCount; i++)
    {
      const TextureInfo &tex = textures[i];
      const Extent2D ext = {tex.width, tex.height};
      imagesIn[i] = {format, ext,
                     ImageUsageFlagBits::eTransferDst |
                         ImageUsageFlagBits::eSampled};
    }

    const auto internalImageIndex = createInternalImages(this, imagesIn);

    for (size_t i = 0; i < textureCount; i++)
    {
      const TextureInfo &tex = textures[i];
      const Extent3D ext = {tex.width, tex.height, 1};

      const BufferCreateInfo intermBufferCreate({}, tex.size,
                                                BufferUsageFlagBits::eTransferSrc,
                                                SharingMode::eExclusive, {});
      const auto intermBuffer = device.createBuffer(intermBufferCreate);
      intermBuffers.push_back(intermBuffer);
      const auto memRequIntern = device.getBufferMemoryRequirements(intermBuffer);
      const MemoryAllocateInfo intermMemAllocInfo(memRequIntern.size,
                                                  memoryTypeHostVisibleCoherent);
      const auto intermMemory = device.allocateMemory(intermMemAllocInfo);
      intermMemorys.push_back(intermMemory);
      device.bindBufferMemory(intermBuffer, intermMemory, 0);
      const auto handle = device.mapMemory(intermMemory, 0, VK_WHOLE_SIZE, {});
      std::memcpy(handle, tex.data, tex.size);
      device.unmapMemory(intermMemory);

      intermCopys.push_back({0,
                             tex.width,
                             tex.height,
                             {ImageAspectFlagBits::eColor, 0, 0, 1},
                             {},
                             ext});

      const auto curentImg = textureImages[i + internalImageIndex];

      waitForImageTransition(
          cmd, ImageLayout::eUndefined, ImageLayout::eTransferDstOptimal,
          curentImg, range, PipelineStageFlagBits::eTopOfPipe,
          AccessFlagBits::eNoneKHR, PipelineStageFlagBits::eTransfer,
          AccessFlagBits::eTransferWrite);

      cmd.copyBufferToImage(intermBuffer, curentImg,
                            ImageLayout::eTransferDstOptimal, intermCopys.back());

      waitForImageTransition(
          cmd, ImageLayout::eTransferDstOptimal,
          ImageLayout::eShaderReadOnlyOptimal, curentImg, range,
          PipelineStageFlagBits::eTransfer, AccessFlagBits::eTransferWrite,
          PipelineStageFlagBits::eFragmentShader, AccessFlagBits::eShaderRead);
    }

    cmd.end();

    const SubmitInfo submitInfo({}, {}, cmd, {});
    queue.submit(submitInfo, commandBufferFence);
    const Result result =
        device.waitForFences(commandBufferFence, true, UINT64_MAX);
    VERROR(result);
    device.resetFences(commandBufferFence);

    return firstIndex;
  }

#ifdef DEBUG
  VkBool32 debugMessage(DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                        DebugUtilsMessageTypeFlagsEXT messageTypes,
                        const DebugUtilsMessengerCallbackDataEXT *pCallbackData,
                        void *pUserData)
  {
    if (messageSeverity == DebugUtilsMessageSeverityFlagBitsEXT::eVerbose)
    {
      return VK_TRUE;
    }
    std::string severity = to_string(messageSeverity);
    std::string type = to_string(messageTypes);

    printf("[%s][%s]: %s\n", severity.c_str(), type.c_str(),
           pCallbackData->pMessage);
    return !(bool)(messageSeverity |
                   DebugUtilsMessageSeverityFlagBitsEXT::eError);
  }
#endif

  inline void createLightPass(VulkanGraphicsModule *vgm)
  {

    const auto sapi = vgm->getShaderAPI();

    const auto pipe = (VulkanShaderPipe *)sapi->loadShaderPipeAndCompile(
        {"assets/lightPass.vert", "assets/lightPass.frag"});
    vgm->shaderPipes.push_back(pipe);
    vgm->lightBindings = sapi->createBindings(pipe, 1);

    vgm->lights.lightCount = 1;
    vgm->lights.lights[0] = Light(glm::vec3(4, 4, 4), glm::vec3(1, 1, 1), 0.2f);
    vgm->lights.lights[1] = Light(glm::vec3(-10, 4, 4), glm::vec3(1, 1, 1), 0.4f);

    auto ptr = (const uint8_t *)&vgm->lights;
    auto sizeOfLight = sizeof(vgm->lights);
    const auto dataID = vgm->pushData(1, &ptr, &sizeOfLight, DataType::Uniform);

    const std::array bindingInfos = {
        BindingInfo{0,
                    vgm->lightBindings,
                    BindingType::InputAttachment,
                    {vgm->albedoImage, UINT64_MAX}},
        BindingInfo{1,
                    vgm->lightBindings,
                    BindingType::InputAttachment,
                    {vgm->normalImage, UINT64_MAX}},
        BindingInfo{2,
                    vgm->lightBindings,
                    BindingType::InputAttachment,
                    {vgm->roughnessMetallicImage, UINT64_MAX}},
        BindingInfo{3,
                    vgm->lightBindings,
                    BindingType::InputAttachment,
                    {vgm->position, UINT64_MAX}},
        BindingInfo{4,
                    vgm->lightBindings,
                    BindingType::UniformBuffer,
                    {dataID, VK_WHOLE_SIZE, 0}}};

    sapi->bindData(bindingInfos.data(), bindingInfos.size());

    for (const auto &shaderPair : pipe->shader)
    {
      const auto &shaderData = shaderPair.first;

      const ShaderModuleCreateInfo shaderModuleCreateInfo(
          {}, shaderData.size() * sizeof(uint32_t), shaderData.data());
      const auto shaderModule =
          vgm->device.createShaderModule(shaderModuleCreateInfo);
      vgm->shaderModules.push_back(shaderModule);
      pipe->pipelineShaderStage.push_back(PipelineShaderStageCreateInfo(
          {}, shaderPair.second, shaderModule, "main"));
    }

    const Rect2D sic = {
        {0, 0}, {(uint32_t)vgm->viewport.width, (uint32_t)vgm->viewport.height}};

    const PipelineVertexInputStateCreateInfo visci;
    const PipelineViewportStateCreateInfo vsci({}, vgm->viewport, sic);
    const PipelineRasterizationStateCreateInfo rsci(
        {}, false, false, {}, {}, {}, false, 0.0f, 0.0f, 0.0f, 1.0f);

    const PipelineMultisampleStateCreateInfo msci;

    constexpr std::array blendAttachment = {PipelineColorBlendAttachmentState(
        true, BlendFactor::eSrcAlpha, BlendFactor::eOneMinusSrcAlpha,
        BlendOp::eAdd, BlendFactor::eOne, BlendFactor::eZero, BlendOp::eAdd,
        (ColorComponentFlags)FlagTraits<ColorComponentFlagBits>::allFlags)};

    const PipelineColorBlendStateCreateInfo colorBlendState(
        {}, false, LogicOp::eOr, blendAttachment);

    GraphicsPipelineCreateInfo graphicsPipeline(
        {}, pipe->pipelineShaderStage, &visci, &inputAssemblyCreateInfo, {},
        &vsci, &rsci, &msci, {}, &colorBlendState, nullptr, nullptr,
        vgm->renderpass, 1);
    vgm->lightMat = Material(pipe);
    sapi->addToMaterial(&vgm->lightMat, &graphicsPipeline);

    const auto gp = vgm->device.createGraphicsPipeline({}, graphicsPipeline);
    VERROR(gp.result)
    vgm->lightPipe = vgm->pipelines.size();
    vgm->pipelines.push_back(gp.value);
  }

  main::Error VulkanGraphicsModule::init()
  {
    this->shaderAPI = new VulkanShaderModule(this);
#pragma region Instance
    const ApplicationInfo applicationInfo(APPLICATION_NAME, APPLICATION_VERSION,
                                          ENGINE_NAME, ENGINE_VERSION,
                                          VK_API_VERSION_1_0);

    const auto layerInfos = enumerateInstanceLayerProperties();
    std::vector<const char *> layerEnabled;
    for (const auto &layer : layerInfos)
    {
      const auto lname = layer.layerName.data();
      const auto enditr = layerToEnable.end();
      if (std::find_if(layerToEnable.begin(), enditr,
                       [&](auto in)
                       { return strcmp(lname, in) == 0; }) != enditr)
        layerEnabled.push_back(lname);
    }

    const auto extensionInfos = enumerateInstanceExtensionProperties();
    std::vector<const char *> extensionEnabled;
    for (const auto &extension : extensionInfos)
    {
      const auto lname = extension.extensionName.data();
      const auto enditr = extensionToEnable.end();
      if (std::find_if(extensionToEnable.begin(), enditr,
                       [&](auto in)
                       { return strcmp(lname, in) == 0; }) != enditr)
        extensionEnabled.push_back(lname);
    }

    const InstanceCreateInfo createInfo(
        {}, &applicationInfo, (uint32_t)layerEnabled.size(), layerEnabled.data(),
        (uint32_t)extensionEnabled.size(), extensionEnabled.data());
    this->instance = createInstance(createInfo);

#ifdef DEBUG
    DispatchLoaderDynamic stat;
    stat.vkCreateDebugUtilsMessengerEXT =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkCreateDebugUtilsMessengerEXT");
    const DebugUtilsMessengerCreateInfoEXT debugUtilsMsgCreateInfo(
        {},
        (DebugUtilsMessageSeverityFlagsEXT)
            FlagTraits<DebugUtilsMessageSeverityFlagBitsEXT>::allFlags,
        (DebugUtilsMessageTypeFlagsEXT)
            FlagTraits<DebugUtilsMessageTypeFlagBitsEXT>::allFlags,
        (PFN_vkDebugUtilsMessengerCallbackEXT)debugMessage);
    debugMessenger = instance.createDebugUtilsMessengerEXT(
        debugUtilsMsgCreateInfo, nullptr, stat);
#endif
#pragma endregion

#pragma region Device
    constexpr auto getScore = [](auto physDevice)
    {
      const auto properties = physDevice.getProperties();
      return properties.limits.maxImageDimension2D +
             (properties.deviceType == PhysicalDeviceType::eDiscreteGpu ? 1000
                                                                        : 0);
    };

    const auto physicalDevices = this->instance.enumeratePhysicalDevices();
    this->physicalDevice = *std::max_element(
        physicalDevices.begin(), physicalDevices.end(),
        [&](auto p1, auto p2)
        { return getScore(p1) < getScore(p2); });

    // just one queue for now
    const auto queueFamilys = this->physicalDevice.getQueueFamilyProperties();
    const auto bgnitr = queueFamilys.begin();
    const auto enditr = queueFamilys.end();
    const auto queueFamilyItr = std::find_if(bgnitr, enditr, [](auto queue)
                                             { return queue.queueFlags & QueueFlagBits::eGraphics; });
    if (queueFamilyItr == enditr)
      return main::Error::NO_GRAPHIC_QUEUE_FOUND;

    queueFamilyIndex = (uint32_t)std::distance(bgnitr, queueFamilyItr);
    const auto &queueFamily = *queueFamilyItr;
    std::vector<float> priorities(queueFamily.queueCount);
    std::fill(priorities.begin(), priorities.end(), 0.0f);

    queueIndex = (uint32_t)std::distance(bgnitr, queueFamilyItr);
    const DeviceQueueCreateInfo queueCreateInfo(
        {}, queueIndex, queueFamily.queueCount, priorities.data());

    const auto devextensions =
        physicalDevice.enumerateDeviceExtensionProperties();
    const auto devextEndItr = devextensions.end();
    const auto fndDevExtItr = std::find_if(
        devextensions.begin(), devextEndItr, [](ExtensionProperties prop)
        { return strcmp(prop.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0; });
    if (fndDevExtItr == devextEndItr)
      return main::Error::SWAPCHAIN_EXT_NOT_FOUND;

    const char *name = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    const DeviceCreateInfo deviceCreateInfo({}, 1, &queueCreateInfo, 0, {}, 1,
                                            &name);
    this->device = this->physicalDevice.createDevice(deviceCreateInfo);

    const auto c4Props =
        this->physicalDevice.getFormatProperties(Format::eR8G8B8A8Unorm);
    if (!(c4Props.optimalTilingFeatures &
          FormatFeatureFlagBits::eColorAttachment))
      return main::Error::FORMAT_NOT_SUPPORTED;

#pragma endregion

#pragma region Queue, Surface, Prepipe, MemTypes
    queue = device.getQueue(queueFamilyIndex, queueIndex);

    const auto winM = graphicsModule->getWindowModule();
#ifdef WIN32
    Win32SurfaceCreateInfoKHR surfaceCreateInfo({}, (HINSTANCE)winM->hInstance,
                                                (HWND)winM->hWnd);
    surface = instance.createWin32SurfaceKHR(surfaceCreateInfo);
#endif // WIN32

    if (!physicalDevice.getSurfaceSupportKHR(queueIndex, surface))
      return main::Error::NO_SURFACE_SUPPORT;

    const auto surfaceFormat = physicalDevice.getSurfaceFormatsKHR(surface);
    const auto surfEndItr = surfaceFormat.end();
    const auto surfBeginItr = surfaceFormat.begin();
    const auto fitr =
        std::find_if(surfBeginItr, surfEndItr, [](SurfaceFormatKHR format)
                     { return format.format == Format::eB8G8R8A8Unorm; });
    if (fitr == surfEndItr)
      return main::Error::FORMAT_NOT_FOUND;
    format = *fitr;

    const auto memoryProperties = physicalDevice.getMemoryProperties();
    const auto memBeginItr = memoryProperties.memoryTypes.begin();
    const auto memEndItr = memoryProperties.memoryTypes.end();

    const auto findMemoryIndex = [&](auto prop)
    {
      const auto findItr = std::find_if(memBeginItr, memEndItr, [&](auto &type)
                                        { return type.propertyFlags & (prop); });
      return std::distance(memBeginItr, findItr);
    };

    memoryTypeDeviceLocal = findMemoryIndex(MemoryPropertyFlagBits::eDeviceLocal);
    memoryTypeHostVisibleCoherent =
        findMemoryIndex(MemoryPropertyFlagBits::eHostVisible |
                        MemoryPropertyFlagBits::eHostCoherent);

    const auto capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
    viewport = Viewport(0, 0, capabilities.currentExtent.width,
                        capabilities.currentExtent.height, 0, 1.0f);

    const auto presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
    const auto presentModesEndItr = presentModes.end();
    const auto presentModesBeginItr = presentModes.begin();
    auto fndPresentMode = std::find(presentModesBeginItr, presentModesEndItr,
                                    PresentModeKHR::eMailbox);
    if (fndPresentMode == presentModesEndItr)
    {
      fndPresentMode = std::find(presentModesBeginItr, presentModesEndItr,
                                 PresentModeKHR::eImmediate);
      if (fndPresentMode == presentModesEndItr)
        fndPresentMode = presentModesBeginItr;
    }
    const auto presentMode = *fndPresentMode;
#pragma endregion

#pragma region Swapchain
    const SwapchainCreateInfoKHR swapchainCreateInfo(
        {}, surface, 3, format.format, format.colorSpace,
        capabilities.currentExtent, 1, ImageUsageFlagBits::eColorAttachment,
        SharingMode::eExclusive, 0, nullptr,
        SurfaceTransformFlagBitsKHR::eIdentity,
        CompositeAlphaFlagBitsKHR::eOpaque, presentMode, true, nullptr);

    swapchain = device.createSwapchainKHR(swapchainCreateInfo);

    swapchainImages = device.getSwapchainImagesKHR(swapchain);
#pragma endregion

#pragma region Depth and Output Attachments
    constexpr std::array potentialDepthFormat = {
        Format::eD32Sfloat, Format::eD32SfloatS8Uint, Format::eD24UnormS8Uint,
        Format::eD16Unorm, Format::eD16UnormS8Uint};
    for (const Format pDF : potentialDepthFormat)
    {
      const FormatProperties fProp = physicalDevice.getFormatProperties(pDF);
      if (fProp.optimalTilingFeatures &
          FormatFeatureFlagBits::eDepthStencilAttachment)
      {
        depthFormat = pDF;
        break;
      }
    }
    if (depthFormat == Format::eUndefined)
      return main::Error::FORMAT_NOT_FOUND;
    const Extent2D ext = {(uint32_t)viewport.width, (uint32_t)viewport.height};
    const std::vector<InternalImageInfo> intImageInfo = {
        {depthFormat, ext, ImageUsageFlagBits::eDepthStencilAttachment},
        {format.format, ext,
         ImageUsageFlagBits::eColorAttachment |
             ImageUsageFlagBits::eInputAttachment},
        {Format::eR8G8B8A8Snorm, ext,
         ImageUsageFlagBits::eColorAttachment |
             ImageUsageFlagBits::eInputAttachment},
        {Format::eR8G8Snorm, ext,
         ImageUsageFlagBits::eColorAttachment |
             ImageUsageFlagBits::eInputAttachment},
        {Format::eR8G8B8A8Snorm, ext,
         ImageUsageFlagBits::eColorAttachment |
             ImageUsageFlagBits::eInputAttachment}};

    const auto imageFirstIndex = createInternalImages(this, intImageInfo);
    depthImage = imageFirstIndex;
    albedoImage = imageFirstIndex + 1;
    normalImage = imageFirstIndex + 2;
    roughnessMetallicImage = imageFirstIndex + 3;
    position = imageFirstIndex + 4;
#pragma endregion

#pragma region Renderpass

    const std::array attachments = {
        AttachmentDescription(
            {}, depthFormat, SampleCountFlagBits::e1, AttachmentLoadOp::eClear,
            AttachmentStoreOp::eDontCare, AttachmentLoadOp::eDontCare,
            AttachmentStoreOp::eDontCare, ImageLayout::eUndefined,
            ImageLayout::eDepthStencilAttachmentOptimal),
        AttachmentDescription(
            {}, format.format, SampleCountFlagBits::e1, AttachmentLoadOp::eClear,
            AttachmentStoreOp::eStore, AttachmentLoadOp::eDontCare,
            AttachmentStoreOp::eDontCare, ImageLayout::eUndefined,
            ImageLayout::eSharedPresentKHR),
        AttachmentDescription(
            {}, intImageInfo[2].format, SampleCountFlagBits::e1,
            AttachmentLoadOp::eClear, AttachmentStoreOp::eDontCare,
            AttachmentLoadOp::eDontCare, AttachmentStoreOp::eDontCare,
            ImageLayout::eUndefined, ImageLayout::eSharedPresentKHR),
        AttachmentDescription(
            {}, intImageInfo[3].format, SampleCountFlagBits::e1,
            AttachmentLoadOp::eClear, AttachmentStoreOp::eDontCare,
            AttachmentLoadOp::eDontCare, AttachmentStoreOp::eDontCare,
            ImageLayout::eUndefined, ImageLayout::eSharedPresentKHR),
        AttachmentDescription(
            {}, intImageInfo[4].format, SampleCountFlagBits::e1,
            AttachmentLoadOp::eClear, AttachmentStoreOp::eDontCare,
            AttachmentLoadOp::eDontCare, AttachmentStoreOp::eDontCare,
            ImageLayout::eUndefined, ImageLayout::eSharedPresentKHR),
        AttachmentDescription(
            {}, format.format, SampleCountFlagBits::e1, AttachmentLoadOp::eClear,
            AttachmentStoreOp::eStore, AttachmentLoadOp::eDontCare,
            AttachmentStoreOp::eDontCare, ImageLayout::eUndefined,
            ImageLayout::eColorAttachmentOptimal)};

    constexpr std::array colorAttachments = {
        AttachmentReference(1, ImageLayout::eColorAttachmentOptimal),
        AttachmentReference(2, ImageLayout::eColorAttachmentOptimal),
        AttachmentReference(3, ImageLayout::eColorAttachmentOptimal),
        AttachmentReference(4, ImageLayout::eColorAttachmentOptimal)};

    constexpr std::array inputAttachments = {
        AttachmentReference(1, ImageLayout::eShaderReadOnlyOptimal),
        AttachmentReference(2, ImageLayout::eShaderReadOnlyOptimal),
        AttachmentReference(3, ImageLayout::eShaderReadOnlyOptimal),
        AttachmentReference(4, ImageLayout::eShaderReadOnlyOptimal)};

    constexpr std::array colorAttachmentsSubpass1 = {
        AttachmentReference(5, ImageLayout::eColorAttachmentOptimal)};

    constexpr AttachmentReference depthAttachment(
        0, ImageLayout::eDepthStencilAttachmentOptimal);

    const std::array subpassDescriptions = {
        SubpassDescription({}, PipelineBindPoint::eGraphics, {}, colorAttachments,
                           {}, &depthAttachment),
        SubpassDescription({}, PipelineBindPoint::eGraphics, inputAttachments,
                           colorAttachmentsSubpass1)};

    const auto frag1 = PipelineStageFlagBits::eColorAttachmentOutput |
                       PipelineStageFlagBits::eEarlyFragmentTests;

    const auto frag2 = AccessFlagBits::eColorAttachmentWrite |
                       AccessFlagBits::eColorAttachmentRead |
                       AccessFlagBits::eDepthStencilAttachmentRead |
                       AccessFlagBits::eDepthStencilAttachmentWrite;

    const std::array subpassDependencies = {
        SubpassDependency(0, 1, frag1, frag1, frag2, frag2),
        SubpassDependency(1, VK_SUBPASS_EXTERNAL, frag1, frag1, frag2, frag2)};

    const RenderPassCreateInfo renderPassCreateInfo(
        {}, attachments, subpassDescriptions, subpassDependencies);
    renderpass = device.createRenderPass(renderPassCreateInfo);
#pragma endregion

#pragma region CommandBuffer
    swapchainImageviews.reserve(swapchainImages.size());

    const CommandPoolCreateInfo commandPoolCreateInfo(
        CommandPoolCreateFlagBits::eResetCommandBuffer, queueIndex);
    pool = device.createCommandPool(commandPoolCreateInfo);

    const CommandBufferAllocateInfo cmdBufferAllocInfo(
        pool, CommandBufferLevel::ePrimary, (uint32_t)swapchainImages.size() + 1);
    cmdbuffer = device.allocateCommandBuffers(cmdBufferAllocInfo);
#pragma endregion

#pragma region ImageViews and Framebuffer
    for (auto im : swapchainImages)
    {
      const ImageViewCreateInfo imageviewCreateInfo(
          {}, im, ImageViewType::e2D, format.format, ComponentMapping(),
          ImageSubresourceRange(ImageAspectFlagBits::eColor, 0, 1, 0, 1));

      const auto imview = device.createImageView(imageviewCreateInfo);
      swapchainImageviews.push_back(imview);

      std::array<ImageView, attachments.size()> images;
      std::copy(textureImageViews.begin() + imageFirstIndex,
                textureImageViews.begin() + imageFirstIndex + intImageInfo.size(),
                images.begin());
      images.back() = imview;

      const FramebufferCreateInfo framebufferCreateInfo(
          {}, renderpass, images, viewport.width, viewport.height, 1);
      framebuffer.push_back(device.createFramebuffer(framebufferCreateInfo));
    }
#pragma endregion

#pragma region Vulkan Mutex
    const FenceCreateInfo fenceCreateInfo;
    commandBufferFence = device.createFence(fenceCreateInfo);

    const SemaphoreCreateInfo semaphoreCreateInfo;
    waitSemaphore = device.createSemaphore(semaphoreCreateInfo);
    signalSemaphore = device.createSemaphore(semaphoreCreateInfo);
#pragma endregion

    this->isInitialiazed = true;
    this->shaderAPI->init();
    device.waitIdle();

    createLightPass(this);

    auto nextimage =
        device.acquireNextImageKHR(swapchain, UINT64_MAX, waitSemaphore, {});
    VERROR(nextimage.result);
    this->nextImage = nextimage.value;

    return main::Error::NONE;
  }

  void VulkanGraphicsModule::tick(double time)
  {
    if (exitFailed)
      return;

    const auto currentBuffer = cmdbuffer[this->nextImage];
    if (1)
    { // For now rerecord every tick
      constexpr std::array clearColor = {1.0f, 1.0f, 1.0f, 1.0f};
      const std::array clearValue = {ClearValue(ClearDepthStencilValue(1.0f, 0)),
                                     ClearValue(clearColor),
                                     ClearValue(clearColor),
                                     ClearValue(clearColor),
                                     ClearValue(clearColor),
                                     ClearValue(clearColor),
                                     ClearValue(clearColor)};

      const CommandBufferBeginInfo cmdBufferBeginInfo({}, nullptr);
      currentBuffer.begin(cmdBufferBeginInfo);

      const RenderPassBeginInfo renderPassBeginInfo(
          renderpass, framebuffer[this->nextImage],
          {{0, 0}, {(uint32_t)viewport.width, (uint32_t)viewport.height}},
          clearValue);
      currentBuffer.beginRenderPass(renderPassBeginInfo,
                                    SubpassContents::eSecondaryCommandBuffers);
      const std::lock_guard onExitUnlock(commandBufferRecording);
      currentBuffer.executeCommands(secondaryCommandBuffer);

      currentBuffer.nextSubpass(SubpassContents::eInline);

      currentBuffer.bindPipeline(PipelineBindPoint::eGraphics,
                                 pipelines[lightPipe]);

      const std::array lights = {lightBindings};
      getShaderAPI()->addToRender(lights.data(), lights.size(),
                                  (CommandBuffer *)&currentBuffer);

      currentBuffer.draw(3, 1, 0, 0);

      currentBuffer.endRenderPass();
      currentBuffer.end();
    }

    primary[0] = currentBuffer;

    const PipelineStageFlags stageFlag =
        PipelineStageFlagBits::eColorAttachmentOutput |
        PipelineStageFlagBits::eLateFragmentTests;
    const SubmitInfo submitInfo(waitSemaphore, stageFlag, primary,
                                signalSemaphore);

    queue.submit(submitInfo, commandBufferFence);

    const PresentInfoKHR presentInfo(signalSemaphore, swapchain, this->nextImage,
                                     nullptr);
    const Result result = queue.presentKHR(&presentInfo);
    if (result == Result::eErrorOutOfDateKHR)
    {
      exitFailed = true;
      return;
    }
    if (result == Result::eErrorInitializationFailed)
    {
      printf("For some reasone NV drivers seem to be hitting this error!");
    }
    VERROR(result);

    const Result waitresult =
        device.waitForFences(commandBufferFence, true, UINT64_MAX);
    VERROR(waitresult);

    currentBuffer.reset();
    device.resetFences(commandBufferFence);

    auto nextimage =
        device.acquireNextImageKHR(swapchain, UINT64_MAX, waitSemaphore, {});
    VERROR(nextimage.result);
    this->nextImage = nextimage.value;
  }

  void VulkanGraphicsModule::destroy()
  {
    this->isInitialiazed = false;
    device.waitIdle();
    this->shaderAPI->destroy();
    device.destroyFence(commandBufferFence);
    device.destroySemaphore(waitSemaphore);
    device.destroySemaphore(signalSemaphore);
    device.freeCommandBuffers(pool, secondaryCommandBuffer);
    for (const auto imag : textureImages)
      device.destroyImage(imag);
    const auto eItr = std::unique(
        textureMemorys.begin(), textureMemorys.end(),
        [](auto t1, auto t2)
        { return std::get<0>(t1) == std::get<0>(t2); });
    for (auto itr = textureMemorys.begin(); itr != eItr; itr++)
      device.freeMemory(std::get<0>(*itr));
    for (const auto imView : textureImageViews)
      device.destroyImageView(imView);
    for (const auto samp : sampler)
      device.destroySampler(samp);
    for (const auto mem : bufferMemoryList)
      device.freeMemory(mem);
    for (const auto buf : bufferList)
      device.destroyBuffer(buf);
    for (const auto pipe : pipelines)
      device.destroyPipeline(pipe);
    for (const auto shader : shaderModules)
      device.destroyShaderModule(shader);
    device.freeCommandBuffers(pool, cmdbuffer);
    device.destroyCommandPool(pool);
    for (const auto framebuff : framebuffer)
      device.destroyFramebuffer(framebuff);
    for (const auto imv : swapchainImageviews)
      device.destroyImageView(imv);
    device.destroyRenderPass(renderpass);
    device.destroySwapchainKHR(swapchain);
    device.destroy();
    instance.destroySurfaceKHR(surface);
#ifdef DEBUG
    DispatchLoaderDynamic stat;
    stat.vkDestroyDebugUtilsMessengerEXT =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkDestroyDebugUtilsMessengerEXT");
    instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, stat);
#endif
    instance.destroy();
    delete shaderAPI;
  }

  APILayer *getNewVulkanModule() { return new VulkanGraphicsModule(); }

} // namespace tge::graphics
