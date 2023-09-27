#pragma once
#include <memory>
#include <vector>
#include <RoxEngine/renderer/RenderPass.h>
#include <vulkan/vulkan.hpp>

namespace RoxEngine::Vulkan {
	class RenderPass : public RoxEngine::RenderPass
	{
	public:
		RenderPass(const std::vector<AttachmentDescription>& attachments, const std::vector<Subpass>& subpasses);
		virtual const std::vector<AttachmentDescription>& GetAttachments();
		virtual const std::vector<Subpass>& GetSubpasses();

		vk::RenderPass mRenderPass;
		std::vector<AttachmentDescription> mAttachments;
		std::vector<Subpass> mSubpasses;


	};
}

