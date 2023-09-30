#pragma once
#include <RoxEngine/renderer/Framebuffer.h>
#include <vulkan/vulkan.hpp>
namespace RoxEngine::Vulkan {
	class Framebuffer : public RoxEngine::Framebuffer
	{
	public:
		// Only used for swapchain framebuffer
		Framebuffer(vk::Framebuffer framebuffer, vk::Image img, vk::ImageView imgview,uint32_t width, uint32_t height);
		Framebuffer(uint32_t width, uint32_t height, std::vector<FramebufferTexFormat> attachments, std::shared_ptr<RenderPass> renderpass);
		~Framebuffer();
		std::shared_ptr<RoxEngine::RenderPass> GetRenderPass() override;
		glm::ivec2 GetSize() override;

		glm::ivec2 mSize;
		vk::Framebuffer mFramebuffer;
		std::shared_ptr<RoxEngine::RenderPass> mRenderPass;
		std::vector<std::shared_ptr<RenderTexture>> mColorAttachments;
		vk::Image mImg;
		std::vector<vk::ImageView> mColorAttachmentViews;
		bool mSwapchain;
	};
}

