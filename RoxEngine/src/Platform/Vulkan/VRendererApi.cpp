#include "VRendererApi.h"
#include <RoxEngine/core/Assert.h>
#include <GLFW/glfw3.h>
#include <set>

#include "VImGuiLayer.h"
#include <imgui/backends/imgui_impl_vulkan.h>

#include "VCommandBuffer.h"
#include "VRenderTexture.h"

#ifdef VULKAN_DEBUG

PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pMessenger)
{
	return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks const* pAllocator)
{
	return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
	VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
	void* /*pUserData*/)
{
	std::ostringstream message;

	message << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) << ": "
		<< vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) << ":\n";
	message << std::string("\t") << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
	message << std::string("\t") << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
	message << std::string("\t") << "message         = <" << pCallbackData->pMessage << ">\n";
	if (0 < pCallbackData->queueLabelCount)
	{
		message << std::string("\t") << "Queue Labels:\n";
		for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++)
		{
			message << std::string("\t\t") << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
		}
	}
	if (0 < pCallbackData->cmdBufLabelCount)
	{
		message << std::string("\t") << "CommandBuffer Labels:\n";
		for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
		{
			message << std::string("\t\t") << "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
		}
	}
	if (0 < pCallbackData->objectCount)
	{
		message << std::string("\t") << "Objects:\n";
		for (uint32_t i = 0; i < pCallbackData->objectCount; i++)
		{
			message << std::string("\t\t") << "Object " << i << "\n";
			message << std::string("\t\t\t") << "objectType   = " << vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)) << "\n";
			message << std::string("\t\t\t") << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
			if (pCallbackData->pObjects[i].pObjectName)
			{
				message << std::string("\t\t\t") << "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
			}
		}
	}
	RE_CORE_INFO(message.str());

	return false;
}
#endif

#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>
#include "VCommandBuffer.h"
#include "VRenderTexture.h"
#include "VFramebuffer.h"

#include <RoxEngine/renderer/Graphics.h>

namespace RoxEngine::Vulkan {
	inline std::vector<const char*> GetLayers() {
#ifndef VULKAN_DEBUG
		return {
		};
#else
		return {
			"VK_LAYER_KHRONOS_validation",
			//"VK_LAYER_LUNARG_core_validation",
			//"VK_LAYER_LUNARG_object_tracker",
			//"VK_LAYER_LUNARG_image",
		};
#endif
	}
	inline std::vector<const char*> GetInstanceExtensions() {
		return {
			#ifdef VULKAN_DEBUG
				VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
			#endif
			#ifdef RE_PLATFORM_WINDOWS
				"VK_KHR_win32_surface",
			#endif
			"VK_KHR_surface",
		};
	}
	inline std::vector<const char*> GetDeviceExtensions() {
		return {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
	}

	void RendererApi::chooseDevice()
	{
		std::vector<vk::PhysicalDevice> pdevices = mInstance.enumeratePhysicalDevices();
		std::vector<vk::PhysicalDevice> possible_gpus;

		RE_CORE_ASSERT(pdevices.size() != 0, "No gpu that supports vulkan");

		for (auto& pdevice : pdevices)
		{
			vk::PhysicalDeviceProperties properties = pdevice.getProperties();
			std::vector<vk::QueueFamilyProperties> queueFamilies = pdevice.getQueueFamilyProperties();

			QueueFamilyIndices indices;

			int i = 0;
			for (const auto& queueFamily : queueFamilies)
			{
				if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
					indices.graphicsFamily = i;
				}

				if (pdevice.getSurfaceSupportKHR(i, mSurface)) {
					indices.presentFamily = i;
				}

				if (indices.isComplete()) {
					break;
				}

				i++;
			}
			if (!indices.isComplete())
				continue;
			mQueueFamilyIndices = indices;
			mPhysicalDevice = pdevice;
		}
		vk::PhysicalDeviceProperties deviceProperties = mPhysicalDevice.getProperties();
		RE_CORE_INFO("Using {} device to initialize vulkan", deviceProperties.deviceName.data());
	}
	void RendererApi::createDevice()
	{
		RE_CORE_ASSERT(mPhysicalDevice, "PhysicalDevice was not created");
		std::set<uint32_t> uniqueQueueFamilies = {
			mQueueFamilyIndices.graphicsFamily.value(),
			mQueueFamilyIndices.presentFamily.value()
		};

		float queuePriority = 1.0f;

		std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			deviceQueueCreateInfos.push_back(vk::DeviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamily, 1, &queuePriority));
		}
		auto layers = GetLayers();
		auto extensions = GetDeviceExtensions();

		mDevice = mPhysicalDevice.createDevice(vk::DeviceCreateInfo(vk::DeviceCreateFlags(), deviceQueueCreateInfos, layers, extensions));
	}
	void RendererApi::createSwapchain() {
		// choose extent
		vk::SurfaceCapabilitiesKHR surfaceCapabilities = mPhysicalDevice.getSurfaceCapabilitiesKHR(mSurface);
		if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
		{
			auto size = mWindow->GetSize();
			mSwapchainExtent.width = std::clamp<uint32_t>(size[0], surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.height);
			mSwapchainExtent.height = std::clamp<uint32_t>(size[1], surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.height);
		}
		else
		{
			// If the surface size is defined, the swap chain size must match
			mSwapchainExtent = surfaceCapabilities.currentExtent;
		}
		std::vector<vk::SurfaceFormatKHR> formats = mPhysicalDevice.getSurfaceFormatsKHR(mSurface);
		assert(!formats.empty());
		RE_CORE_ASSERT(!formats.empty(), "getSurfaceFormats() doesnt return any avaible formats");
		mSwapchainFormat = (formats[0].format == vk::Format::eUndefined) ? vk::Format::eB8G8R8A8Unorm : formats[0].format;

		vk::PresentModeKHR swapchainPresentMode = vk::PresentModeKHR::eFifo;

		vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
			? vk::SurfaceTransformFlagBitsKHR::eIdentity
			: surfaceCapabilities.currentTransform;

		vk::CompositeAlphaFlagBitsKHR compositeAlpha =
			(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
			: (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
			: (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit
			: vk::CompositeAlphaFlagBitsKHR::eOpaque;

		vk::SwapchainCreateInfoKHR swapChainCreateInfo(vk::SwapchainCreateFlagsKHR(),
			mSurface,
			surfaceCapabilities.minImageCount,
			mSwapchainFormat,
			vk::ColorSpaceKHR::eSrgbNonlinear,
			mSwapchainExtent,
			1,
			vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc,
			vk::SharingMode::eExclusive,
			{},
			preTransform,
			compositeAlpha,
			swapchainPresentMode,
			true,
			nullptr);
		uint32_t queueFamilyIndices[2] = { mQueueFamilyIndices.graphicsFamily.value(), mQueueFamilyIndices.presentFamily.value() };
		if (queueFamilyIndices[0] != queueFamilyIndices[1])
		{
			// If the graphics and present queues are from different queue families, we either have to explicitly transfer
			// ownership of images between the queues, or we have to create the swapchain with imageSharingMode as
			// VK_SHARING_MODE_CONCURRENT
			swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			swapChainCreateInfo.queueFamilyIndexCount = 2;
			swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		mSwapchain = mDevice.createSwapchainKHR(swapChainCreateInfo);

		mSwapchainImages = mDevice.getSwapchainImagesKHR(mSwapchain);

		mSwapchainImageViews.reserve(mSwapchainImages.size());
		vk::ImageViewCreateInfo imageViewCreateInfo({}, {}, vk::ImageViewType::e2D, mSwapchainFormat, {}, { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 });
		for (auto& image : mSwapchainImages)
		{
			imageViewCreateInfo.image = image;
			mSwapchainImageViews.push_back(mDevice.createImageView(imageViewCreateInfo));
		}
	}
	void RendererApi::createFramebuffers()
	{
		vk::AttachmentDescription colorAttachment(
			{},                                     // flags
			mSwapchainFormat,                            // format
			vk::SampleCountFlagBits::e1,            // samples
			vk::AttachmentLoadOp::eClear,           // loadOp
			vk::AttachmentStoreOp::eStore,          // storeOp
			vk::AttachmentLoadOp::eDontCare,        // stencilLoadOp
			vk::AttachmentStoreOp::eDontCare,       // stencilStoreOp
			vk::ImageLayout::eUndefined,            // initialLayout
			vk::ImageLayout::ePresentSrcKHR         // finalLayout
		);
		vk::AttachmentReference colorAttachmentRef(
			0,                                      // attachment index
			vk::ImageLayout::eColorAttachmentOptimal // layout
		);

		vk::SubpassDescription subpass(
			{},                                     // flags
			vk::PipelineBindPoint::eGraphics,        // pipelineBindPoint
			0,                                      // inputAttachmentCount
			nullptr,                                // inputAttachments
			1,                                      // colorAttachmentCount
			&colorAttachmentRef,                    // colorAttachments
			nullptr,                                // resolveAttachments
			nullptr,                                // depthStencilAttachment
			0,                                      // preserveAttachmentCount
			nullptr                                 // preserveAttachments
		);

		vk::RenderPassCreateInfo renderPassInfo(
			{},                                     // flags
			colorAttachment,                       // attachments
			subpass                                // subpasses
		);

		mRenderPass = mDevice.createRenderPass(renderPassInfo);

		mSwapchainFramebuffers.resize(mSwapchainImageViews.size());
		std::vector<vk::ImageMemoryBarrier> barriers;
		barriers.reserve(mSwapchainFramebuffers.size());
		for (size_t i = 0; i < mSwapchainFramebuffers.size(); i++) {
			vk::FramebufferCreateInfo createInfo({}, mRenderPass, mSwapchainImageViews[i], mSwapchainExtent.width, mSwapchainExtent.height, 1);

			mSwapchainFramebuffers[i] = std::make_shared<Framebuffer>(mDevice.createFramebuffer(createInfo), mSwapchainImages[i], mSwapchainImageViews[i], mSwapchainExtent.width, mSwapchainExtent.height);
			
			vk::ImageMemoryBarrier barrier(
				vk::AccessFlags(),
				vk::AccessFlagBits::eColorAttachmentWrite,
				vk::ImageLayout::eUndefined,
				vk::ImageLayout::ePresentSrcKHR,
				VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
				mSwapchainImages[i],
				vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)
			);
			barriers.push_back(barrier);
		}

		vk::CommandBufferAllocateInfo cmdAllocInfo(mCommandPool, vk::CommandBufferLevel::ePrimary, 1);
		auto cmd = mDevice.allocateCommandBuffers(cmdAllocInfo).front();
		cmd.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
		cmd.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::DependencyFlags(), nullptr, nullptr, barriers);
		cmd.end();
		vk::SubmitInfo submit_info(nullptr, nullptr, cmd, nullptr);
		mGraphicsQueue.submit(submit_info, nullptr);
		mDevice.waitIdle();
	}

	const int FRAMES_IN_FLIGHT = 3;

	RendererApi::RendererApi(Window* window)
	{
		mWindow = window;
		vk::ApplicationInfo appinfo(window->GetDesc().name, VK_MAKE_VERSION(1, 0, 0), "RoxEngine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0);

		auto instance_layers = GetLayers();
		auto instance_extensions = GetInstanceExtensions();

		vk::InstanceCreateInfo createInfo({}, &appinfo,
			instance_layers,
			instance_extensions
		);

		mInstance = vk::createInstance(createInfo);
#ifdef VULKAN_DEBUG
		pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(mInstance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
		pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(mInstance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));

		RE_CORE_ASSERT(pfnVkCreateDebugUtilsMessengerEXT || pfnVkDestroyDebugUtilsMessengerEXT, "Vulkan Error: Could not get DebugUtilsMessengerEXT functions");

		vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
		vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
			vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
		mdebugUtilsMessenger = mInstance.createDebugUtilsMessengerEXT(vk::DebugUtilsMessengerCreateInfoEXT({}, severityFlags, messageTypeFlags, &debugMessageFunc, nullptr,nullptr));
#endif
		{
			VkSurfaceKHR _surface;
			RE_CORE_ASSERT(glfwCreateWindowSurface(mInstance, (GLFWwindow*)window->GetNativeWindow(), nullptr, &_surface) == VK_SUCCESS, "GLFW could not create surface");
			mSurface = _surface;
		}

		chooseDevice();
		createDevice();
		createSwapchain();
		mGraphicsQueue = mDevice.getQueue(mQueueFamilyIndices.graphicsFamily.value(), 0);
		mPresentQueue = mDevice.getQueue(mQueueFamilyIndices.presentFamily.value(), 0);

		vk::SemaphoreCreateInfo semaphoreInfo;
		vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlagBits::eSignaled);
		
		imageAvailableSemaphores.resize(FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(FRAMES_IN_FLIGHT);
		inFlightFences.resize(FRAMES_IN_FLIGHT);
		cmds.resize(FRAMES_IN_FLIGHT);

		for (int i = 0; i < FRAMES_IN_FLIGHT; i++)
		{
			imageAvailableSemaphores[i] = mDevice.createSemaphore(semaphoreInfo);
			renderFinishedSemaphores[i] = mDevice.createSemaphore(semaphoreInfo);
			inFlightFences[i] = mDevice.createFence(fenceInfo);
		}
		mCommandPool = mDevice.createCommandPool(vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer, mQueueFamilyIndices.graphicsFamily.value()));
		mCommandBuffers = mDevice.allocateCommandBuffers(vk::CommandBufferAllocateInfo(mCommandPool, vk::CommandBufferLevel::ePrimary, mSwapchainImages.size()));

		createFramebuffers();
		{
			VmaAllocator rawAlloc;
			
			VmaVulkanFunctions vulkanFunctions = {};
			vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
			vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

			VmaAllocatorCreateInfo allocatorCreateInfo = {};
			allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_0;
			allocatorCreateInfo.physicalDevice = mPhysicalDevice;
			allocatorCreateInfo.device = mDevice;
			allocatorCreateInfo.instance = mInstance;
			allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

			auto res = vmaCreateAllocator(&allocatorCreateInfo, &rawAlloc);
			RE_CORE_ASSERT(res == VK_SUCCESS, "Could not create Allocator");

			mAllocator = rawAlloc;
		}
		mRendererPipeline = nullptr;
	}
	RendererApi::~RendererApi()
	{

	}
	void RendererApi::SwapBuffers()
	{
		if(!mRendererPipeline)
			mRendererPipeline = Graphics::GetRendererPipeline();
		
		if (!cmds[0]) {
			for (int i = 0; i < FRAMES_IN_FLIGHT; i++)
			{
				cmds[i] = std::static_pointer_cast<CommandBuffer>(CommandBuffer::Create());
			}
		}
		
		mDevice.waitForFences(inFlightFences[mCurrentFrame], true, UINT64_MAX);
		mDevice.resetFences(inFlightFences[mCurrentFrame]);
		mImageIndex = mDevice.acquireNextImageKHR(mSwapchain, UINT64_MAX, imageAvailableSemaphores[mCurrentFrame], nullptr).value;

		auto& cmd = cmds[mCurrentFrame];
		// Record cmd

		{
			cmd->Reset();
			auto cmd2 = mRendererPipeline->GetCmd();
			cmd->InlineCmd(cmd2);

#ifdef USE_IMGUI
			cmd->RawCall([&](RoxEngine::CommandBuffer* c, void* cm) -> void {
				auto cmd = *((vk::CommandBuffer*)cm);
				auto clearval = vk::ClearValue(vk::ClearColorValue(0.f, 1.f, 0.f, 1.0f));
				vk::RenderPassBeginInfo info(mRenderPass, ((Framebuffer*)mSwapchainFramebuffers[mImageIndex].get())->mFramebuffer, vk::Rect2D(vk::Offset2D(0, 0), mSwapchainExtent), clearval);
				cmd.beginRenderPass(info, vk::SubpassContents::eInline);
				ImDrawData* draw_data = ImGui::GetDrawData();
				ImGui_ImplVulkan_RenderDrawData(draw_data, cmd);
				cmd.endRenderPass();
			});
#endif
			cmd->CreateCache();
		}

		vk::PipelineStageFlags waitDstStageMask = vk::PipelineStageFlagBits::eAllCommands;
		vk::SubmitInfo submit_info(imageAvailableSemaphores[mCurrentFrame], waitDstStageMask, cmd->mPrimaryBuffer, renderFinishedSemaphores[mCurrentFrame]);
		mGraphicsQueue.submit(submit_info, inFlightFences[mCurrentFrame]);
#ifdef USE_IMGUI
		auto& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
#endif

		vk::Result res;
		vk::PresentInfoKHR presentInfo(renderFinishedSemaphores[mCurrentFrame], mSwapchain, mImageIndex, res);
		mDevice.waitIdle();
		res = mPresentQueue.presentKHR(presentInfo);

		mImageIndex = (mImageIndex + 1) % mSwapchainImages.size();
		mCurrentFrame = (mCurrentFrame + 1) % FRAMES_IN_FLIGHT;
	}
	std::shared_ptr<RoxEngine::Framebuffer> RendererApi::GetFramebuffer()
	{
		return mSwapchainFramebuffers.at(mImageIndex);
	}
}
