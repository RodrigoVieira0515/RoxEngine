#include "VFramebuffer.h"
#include "VRendererApi.h"
#include "VRenderTexture.h"
#include "VRenderPass.h"

namespace RoxEngine::Vulkan 
{
	Framebuffer::Framebuffer(vk::Framebuffer framebuffer, vk::Image img, vk::ImageView imgview, uint32_t width, uint32_t height)
	{
		mSwapchain = true;
		mFramebuffer = framebuffer;
		mSize = { width,height };
		mImg = img;
		mColorAttachmentViews.push_back(imgview);
	}
	Framebuffer::Framebuffer(uint32_t width, uint32_t height, std::vector<FramebufferTexFormat> attachments, std::shared_ptr<RoxEngine::RenderPass> renderpass)
	{
		mSwapchain = false;
		mSize = { width,height };
		mRenderPass = renderpass;
		auto api = (RendererApi*)(RendererApi::Get().get());
		mColorAttachments.reserve(attachments.size());
		for (auto& att : attachments)
		{
			auto tex = RoxEngine::RenderTexture::Create(width, height, att);
			auto vtex = ((RenderTexture*)tex.get());
			auto attdesc = formatToVk(vtex->mInternalFormat, api->mPhysicalDevice);
			vk::ImageViewCreateInfo imageView(
				{},
				vtex->mImage,
				vk::ImageViewType::e2D,
				std::get<0>(attdesc)
			);

			imageView.setSubresourceRange(vk::ImageSubresourceRange(std::get<2>(attdesc) ? vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil : vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

			mColorAttachmentViews.push_back(api->mDevice.createImageView(imageView));
			mColorAttachments.push_back(tex);
		}
	
		vk::FramebufferCreateInfo framebufferInfo({}, ((RenderPass*)renderpass.get())->mRenderPass, mColorAttachmentViews, width, height, 1);
		mFramebuffer = api->mDevice.createFramebuffer(framebufferInfo);
	}

	Framebuffer::~Framebuffer()
	{

	}
	std::shared_ptr<RoxEngine::RenderPass> Framebuffer::GetRenderPass()
	{
		return mRenderPass;
	}
	glm::ivec2 Framebuffer::GetSize()
	{
		return mSize;
	}
}