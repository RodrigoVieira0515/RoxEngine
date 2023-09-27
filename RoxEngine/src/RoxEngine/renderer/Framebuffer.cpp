#include "Framebuffer.h"
#include <RoxEngine/renderer/RendererApi.h>
#include <RoxEngine/core/Assert.h>
#include <Platform/Vulkan/VFramebuffer.h>


namespace RoxEngine {
    std::shared_ptr<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height, std::vector<FramebufferTexFormat> attachments, std::shared_ptr<RenderPass> renderpass)
    {
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::Framebuffer>(width, height, attachments,renderpass);
		default:
			RE_CORE_ASSERT(false, fmt::format("Framebuffer::Create() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
		return nullptr;
    }
}