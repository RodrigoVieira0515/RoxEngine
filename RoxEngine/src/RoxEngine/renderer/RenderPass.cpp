#include "RenderPass.h"
#include <RoxEngine/renderer/RendererApi.h>
#include <RoxEngine/core/Assert.h>

#include <Platform/Vulkan/VRenderPass.h>

namespace RoxEngine {
	Subpass::Subpass(uint32_t dependency, const std::vector<uint32_t>& inputAttachments, const std::vector<uint32_t>& colorAttachments, const std::vector<uint32_t>& preserveAttachments)
		: dependency(dependency)
		, inputAttachments(inputAttachments)
		, colorAttachments(colorAttachments)
		, preserveAttachments(preserveAttachments)
	{

	}

	std::shared_ptr<RenderPass> RenderPass::Create(const std::vector<AttachmentDescription>& attachments, const std::vector<Subpass>& subpasses)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::RenderPass>(attachments, subpasses);
		default:
			RE_CORE_ASSERT(false, fmt::format("RenderPass::Create() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
	}
}