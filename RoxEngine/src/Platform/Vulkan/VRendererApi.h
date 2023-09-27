#pragma once
#include <RoxEngine/renderer/RendererApi.h>
#include <vulkan/vulkan.hpp>
#include <optional>
#include <RoxEngine/core/Logger.h>
#include <vma/vk_mem_alloc.h>
#include <vma-hpp/vk_mem_alloc.hpp>

#define VULKAN_DEBUG

namespace RoxEngine::Vulkan
{
	class CommandBuffer;

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
	class RendererApi : public RoxEngine::RendererApi
	{
	private:
		void chooseDevice();
		void createDevice();
		void createSwapchain();
		void createFramebuffers();
	public:
		RendererApi(Window* window);
		~RendererApi();

		void SwapBuffers() override;

		Window* mWindow;

		vk::Instance mInstance;
		#ifdef VULKAN_DEBUG
				vk::DebugUtilsMessengerEXT mdebugUtilsMessenger;
		#endif
		vk::SurfaceKHR mSurface;
		QueueFamilyIndices mQueueFamilyIndices;
		vk::PhysicalDevice mPhysicalDevice;
		vk::Device mDevice;

		vk::Queue mGraphicsQueue;
		vk::Queue mPresentQueue;

		vk::SwapchainKHR mSwapchain;
		vk::Format mSwapchainFormat;
		vk::Extent2D mSwapchainExtent;
		std::vector<vk::Image> mSwapchainImages;
		std::vector<vk::ImageView> mSwapchainImageViews;

		vk::Semaphore imageAvailableSemaphore;
		vk::Semaphore renderFinishedSemaphore;
		vk::Fence inFlightFence;

		vk::RenderPass mRenderPass;

		std::vector<vk::Framebuffer> mSwapchainFramebuffers;

		vk::CommandPool mCommandPool;
		std::vector<vk::CommandBuffer> mCommandBuffers;

		vma::Allocator mAllocator;

		CommandBuffer* mExecuteCmd = nullptr;
	};
}

