// ConsoleApplication1.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdbase.h"
#include "Console.h"
#include "Preferences.h"
#include <fstream>
#include "Debug.h"
#include "Device.h"
#include "Swapchain.h"

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

vector<char> getBinarys(const string &name) {
	ifstream stream(name,ios::binary | ios::ate);
	vector<char> retun;
	if (stream) {
		size_t siz = stream.tellg();
		retun.resize(siz);
		stream.seekg(0);
		stream.read(retun.data(), siz);
		stream.close();
	} else {
		error("Can't load shader files", -4);
	}
	return retun;
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

	vkGetDeviceQueue(cdevice, 0, 0, &queue);

	auto binVer = getBinarys("vert.spv");
	auto binFrag = getBinarys("frag.spv");

	creatModule(binVer, &vermodule);
	creatModule(binFrag, &fragmodule);

	VkPipelineShaderStageCreateInfo ver_creat_info = {};
	ver_creat_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	ver_creat_info.module = vermodule;
	ver_creat_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	ver_creat_info.pName = "main";
	ver_creat_info.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo frag_creat_info = {};
	frag_creat_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	frag_creat_info.module = fragmodule;
	frag_creat_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	frag_creat_info.pName = "main";
	frag_creat_info.pSpecializationInfo = nullptr;

	vector<VkPipelineShaderStageCreateInfo> shader_array = { ver_creat_info, frag_creat_info };

	VkPipelineVertexInputStateCreateInfo vert_inp_info = {};
	vert_inp_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vert_inp_info.vertexBindingDescriptionCount = 0;
	vert_inp_info.vertexAttributeDescriptionCount = 0;
	vert_inp_info.pVertexBindingDescriptions = nullptr;
	vert_inp_info.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo in_ass_info = {};
	in_ass_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	in_ass_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	in_ass_info.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = window.size.width;
	viewport.height = window.size.height;
	viewport.maxDepth = 1;
	viewport.minDepth = 0;

	VkRect2D rect;
	rect.offset = { 0,0 };
	rect.extent = window.size;

	VkPipelineViewportStateCreateInfo viewport_create_info = {};
	viewport_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_create_info.pScissors = &rect;
	viewport_create_info.pViewports = &viewport;
	viewport_create_info.scissorCount = 1;
	viewport_create_info.viewportCount = 1;

	VkPipelineRasterizationStateCreateInfo rastera_info = {};
	rastera_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rastera_info.depthClampEnable = VK_FALSE;
	rastera_info.rasterizerDiscardEnable = VK_FALSE;
	rastera_info.polygonMode = VK_POLYGON_MODE_FILL;
	rastera_info.cullMode = VK_CULL_MODE_NONE;
	rastera_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rastera_info.depthBiasEnable = VK_FALSE;
	rastera_info.depthBiasConstantFactor = 0;
	rastera_info.depthBiasClamp = 0;
	rastera_info.depthBiasSlopeFactor = 0;
	rastera_info.lineWidth = 1;

	VkPipelineMultisampleStateCreateInfo pipl_multisample = {};
	pipl_multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	pipl_multisample.alphaToCoverageEnable = VK_FALSE;
	pipl_multisample.alphaToOneEnable = VK_FALSE;
	pipl_multisample.minSampleShading = 1;
	pipl_multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	pipl_multisample.sampleShadingEnable = VK_FALSE;
	pipl_multisample.pSampleMask = nullptr;

	VkPipelineColorBlendAttachmentState state;
	state.blendEnable = VK_TRUE;
	state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
	state.colorBlendOp = VK_BLEND_OP_ADD;
	state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	state.alphaBlendOp = VK_BLEND_OP_ADD;
	state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo color_blend_create_info = {};
	color_blend_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blend_create_info.logicOpEnable = VK_FALSE;
	color_blend_create_info.logicOp = VK_LOGIC_OP_NO_OP;
	color_blend_create_info.attachmentCount = 1;
	color_blend_create_info.pAttachments = &state;
	color_blend_create_info.blendConstants[0] = 0;
	color_blend_create_info.blendConstants[1] = 0;
	color_blend_create_info.blendConstants[2] = 0;
	color_blend_create_info.blendConstants[3] = 0;

	VkPipelineLayoutCreateInfo layout_create_info = {};
	layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	handel(vkCreatePipelineLayout(cdevice,&layout_create_info,nullptr, &pipline_layout));

	VkAttachmentDescription att_desc;
	att_desc.flags = 0;
	att_desc.format = PREFERRED_FORMAT;
	att_desc.samples = VK_SAMPLE_COUNT_1_BIT;
	att_desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	att_desc.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	att_desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	att_desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	att_desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	att_desc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference reference_att;
	reference_att.attachment = 0;
	reference_att.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass_desc = {};
	subpass_desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_desc.colorAttachmentCount = 1;
	subpass_desc.pColorAttachments = &reference_att;

	VkSubpassDependency subpass = {};
	subpass.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpass.dstSubpass = 0;
	subpass.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpass.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpass.srcAccessMask = 0;
	subpass.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpass.dependencyFlags = 0;

	VkRenderPassCreateInfo render_pass_create_info = {};
	render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_create_info.attachmentCount = 1;
	render_pass_create_info.pAttachments = &att_desc;
	render_pass_create_info.subpassCount = 1;
	render_pass_create_info.pSubpasses = &subpass_desc;
	render_pass_create_info.dependencyCount = 1;
	render_pass_create_info.pDependencies = &subpass;

	handel(vkCreateRenderPass(cdevice, &render_pass_create_info, nullptr, &render_pass));

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

void shutdown(int i) {
	handel(vkDeviceWaitIdle(cdevice));

	vkDestroySemaphore(cdevice, renderEnd, nullptr);
	vkDestroySemaphore(cdevice, renderAvailable, nullptr);
	vkFreeCommandBuffers(cdevice,commandpool, commandbuffers.size(), commandbuffers.data());
	vkDestroyCommandPool(cdevice, commandpool, nullptr);
	for each (VkFramebuffer buff in framebuffers)
	{
		vkDestroyFramebuffer(cdevice, buff, nullptr);
	}
	vkDestroyPipeline(cdevice, pipeline, nullptr);
	vkDestroyRenderPass(cdevice, render_pass, nullptr);
	vkDestroyPipelineLayout(cdevice, pipline_layout, nullptr);
	vkDestroyShaderModule(cdevice, vermodule, nullptr);
	vkDestroyShaderModule(cdevice, fragmodule, nullptr);
	for each (VkImageView var in currentImage)
	{
		vkDestroyImageView(cdevice, var, nullptr);
	}
	vkDestroySwapchainKHR(cdevice,SwapChain,nullptr);
	vkDestroyDevice(cdevice, nullptr);

	destroyApplictaion(application);
	destroyWindow(window);

	exit(i);
}

int main()
{
	window.autosize = true;
	window.title = "TEngine";
	creatWindow(window);

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