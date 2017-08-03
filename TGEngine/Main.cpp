#include "stdbase.h"
#include "Console.h"
#include "Preferences.h"
#include <fstream>
#include "Debug.h"
#include "Device.h"
#include "Swapchain.h"
#include "Shader.h"
#include "RenderPass.h"

using namespace std;


void debugLayerAndExtensions() {
	uint32_t layer_count = 0;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	vector<VkLayerProperties> props = {};
	props.resize(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, props.data());	

	
	for (int i = 0; i < layer_count; i++)
	{
		VkLayerProperties cprop = props[i];
		cout << "Found layer property " << cprop.layerName << " - " << cprop.description << endl;
		cout << "Impl_Version ";
		printVersion(cprop.implementationVersion);
		cout << endl;
		cout << "Spec_Version ";
		printVersion(cprop.specVersion);
		cout << endl;

		uint32_t ext_count = 0;
		vkEnumerateInstanceExtensionProperties(cprop.layerName,&ext_count, nullptr);

		vector<VkExtensionProperties> extns = {};
		extns.resize(ext_count);
		vkEnumerateInstanceExtensionProperties(cprop.layerName,&ext_count, extns.data());

		for (int g = 0; g < ext_count; g++)
		{
			cout << endl;
			VkExtensionProperties ex = extns[g];
			cout << g + 1 << ". " << ex.extensionName << " - Version: ";
			printVersion(ex.specVersion);
			cout << endl;
		}

		cout << endl;
	}

	cout << "Extensions: " << endl;

	uint32_t ext_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);

	vector<VkExtensionProperties> extns = {};
	extns.resize(ext_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, extns.data());

	for (int g = 0; g < ext_count; g++)
	{
		cout << endl;
		VkExtensionProperties ex = extns[g];
		cout << g + 1 << ". " << ex.extensionName << " - Version: ";
		printVersion(ex.specVersion);
		cout << endl;
	}

	cout << endl;
}

using namespace Options;
using namespace Pipeline;

void initTGEngine() {

	vector<char*> layers = {
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_VALVE_steam_overlay",
		"VK_LAYER_NV_optimus"
	};

	Window window;
	window.autosize = true;
	window.title = "title";
	createWindow(window);

	Application application;
	application.window = &window;
	application.layers_to_enable = layers;
	application.version = VK_MAKE_VERSION(0, 0, 1);
	createApplication(application);

	Device main_device;
	main_device.prefered_format = VK_FORMAT_B8G8R8A8_UNORM;
	main_device.app = application;
	createDevice(main_device);

	Swapchain swapchain;
	swapchain.app = &application;
	swapchain.device = &main_device;
	swapchain.image_count = 3;
	swapchain.image_usage_flag = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchain.sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain.present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
	createSwapchain(swapchain);

	Shader vertex_shader = {};
	vertex_shader.bits = VK_SHADER_STAGE_VERTEX_BIT;
	vertex_shader.name = "vert.spv";
	vertex_shader.device = main_device.device;
	createShader(vertex_shader);

	Shader fragment_shader = {};
	fragment_shader.bits = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragment_shader.name = "frag.spv";
	fragment_shader.device = main_device.device;
	createShader(fragment_shader);

	RenderPass render_pass = {};
	render_pass.device = &main_device;
	render_pass.window = &window;
	createRenderPass(render_pass);

	VkGraphicsPipelineCreateInfo pipline_create_info = {};
	pipline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipline_create_info.stageCount = 2;
	pipline_create_info.pStages = shader_array.data();
	pipline_create_info.pVertexInputState = &vert_inp_info;
	pipline_create_info.pInputAssemblyState = &in_ass_info;
	pipline_create_info.pTessellationState = nullptr;
	pipline_create_info.pViewportState = &viewport_create_info;
	pipline_create_info.pRasterizationState = &rastera_info;
	pipline_create_info.pMultisampleState = &pipl_multisample;
	pipline_create_info.pDepthStencilState = nullptr;
	pipline_create_info.pColorBlendState = &color_blend_create_info;
	pipline_create_info.pDynamicState = nullptr;
	pipline_create_info.layout = pipline_layout;
	pipline_create_info.renderPass = render_pass;
	pipline_create_info.subpass = 0;
	pipline_create_info.basePipelineHandle = VK_NULL_HANDLE;
	pipline_create_info.basePipelineIndex = -1;

	handel(vkCreateGraphicsPipelines(cdevice,VK_NULL_HANDLE,1,&pipline_create_info,nullptr,&pipeline));
	
	framebuffers.resize(image_count);
	for (size_t i = 0; i < image_count; i++)
	{
		VkFramebufferCreateInfo framebuffer_create_info = {};
		framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer_create_info.renderPass = render_pass;
		framebuffer_create_info.attachmentCount = 1;
		framebuffer_create_info.pAttachments = &(currentImage[i]);
		framebuffer_create_info.width = window.size.width;
		framebuffer_create_info.height = window.size.height;
		framebuffer_create_info.layers = 1;

		cout << i + 1 << ". ";
		handel(vkCreateFramebuffer(cdevice, &framebuffer_create_info, nullptr, &(framebuffers[i])));
	}

	VkCommandPoolCreateInfo command_pool_create = {};
	command_pool_create.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_create.queueFamilyIndex = 0;

	handel(vkCreateCommandPool(cdevice, &command_pool_create, nullptr, &commandpool));

	VkCommandBufferAllocateInfo commandbuffer_allocate_info = {};
	commandbuffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandbuffer_allocate_info.commandPool = commandpool;
	commandbuffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandbuffer_allocate_info.commandBufferCount = currentImage.size();

	commandbuffers.resize(currentImage.size());
	handel(vkAllocateCommandBuffers(cdevice,&commandbuffer_allocate_info, commandbuffers.data()));

	VkCommandBufferBeginInfo command_begin_info = {};
	command_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	for (size_t i = 0; i < commandbuffers.size(); i++)
	{
		cout << i + 1 << "." << endl;

		handel(vkBeginCommandBuffer(commandbuffers[i], &command_begin_info));

		VkRenderPassBeginInfo begin_render_pass = {};
		begin_render_pass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		begin_render_pass.renderPass = render_pass;
		begin_render_pass.framebuffer = framebuffers[i];
		begin_render_pass.renderArea = rect;

		VkClearValue clear_color = { 0,0,0,0 };
		begin_render_pass.clearValueCount = 1;
		begin_render_pass.pClearValues = &clear_color;

		vkCmdBeginRenderPass(commandbuffers[i],&begin_render_pass, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandbuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline);

		vkCmdDraw(commandbuffers[i],3,1,0,0);

		vkCmdEndRenderPass(commandbuffers[i]);

		handel(vkEndCommandBuffer(commandbuffers[i]));
	}

	VkSemaphoreCreateInfo semaphore_create_info = {};
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphore_create_info.flags = 0;
	semaphore_create_info.pNext = nullptr;

	handel(vkCreateSemaphore(cdevice, &semaphore_create_info, nullptr, &renderAvailable));
	handel(vkCreateSemaphore(cdevice, &semaphore_create_info, nullptr, &renderEnd));

	destroyShader(vertex_shader);
	destroyShader(fragment_shader);

	destroySwapchain(swapchain);

	destroyDevice(main_device);

	destroyApplictaion(application);

	destroyWindow(window);
}

int draw() {
	glfwPollEvents();
	if (glfwWindowShouldClose(window.window))return 1;
	uint32_t nextimage = 0;
	handel(vkAcquireNextImageKHR(cdevice, SwapChain, numeric_limits<uint64_t>::max(), renderAvailable, VK_NULL_HANDLE, &nextimage));

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = &renderAvailable;
	vector<VkPipelineStageFlags> stage_flags = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submit_info.pWaitDstStageMask = stage_flags.data();
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &(commandbuffers[nextimage]);
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = &renderEnd;

	handel(vkQueueSubmit(queue, nextimage, &submit_info, VK_NULL_HANDLE));

	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = &renderEnd;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &SwapChain;
	present_info.pImageIndices = &nextimage;
	present_info.pResults = nullptr;

	handel(vkQueuePresentKHR(queue, &present_info));

	return 0;
}

int main()
{

	initVulkan();
	int xcv;
	while (true) {
		xcv = draw();
		if (xcv != -1) {
			break;
		}
	}
	shutdown(xcv);
	return 0;
}