#pragma once
#include <RoxEngine/renderer/RendererApi.h>
#include <vulkan/vulkan.hpp>
#include <optional>
#include <RoxEngine/core/Logger.h>
#include <vma/vk_mem_alloc.h>
#include <vma-hpp/vk_mem_alloc.hpp>
#include <RoxEngine/renderer/RendererPipeline.h>

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

		std::shared_ptr<RoxEngine::Framebuffer> GetFramebuffer();
		
		uint32_t mImageIndex = 0;
		uint32_t mCurrentFrame = 0;

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
		std::vector<std::shared_ptr<RoxEngine::Framebuffer>> mSwapchainFramebuffers;


		std::vector<vk::Semaphore> imageAvailableSemaphores;
		std::vector<vk::Semaphore> renderFinishedSemaphores;
		std::vector<vk::Fence> inFlightFences;

		vk::RenderPass mRenderPass;


		vk::CommandPool mCommandPool;
		std::vector<vk::CommandBuffer> mCommandBuffers;

		vma::Allocator mAllocator;

		std::vector<std::shared_ptr<CommandBuffer>> cmds;

		RendererPipeline* mRendererPipeline;
	};
}

