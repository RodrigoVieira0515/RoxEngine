#include "VRenderPass.h"
#include <vulkan/vulkan.hpp>
#include "VRenderTexture.h"
#include <platform/Vulkan/VRendererApi.h>

namespace RoxEngine::Vulkan {

	

	RenderPass::RenderPass(const std::vector<AttachmentDescription>& attachments, const std::vector<Subpass>& subpasses)
	{
		auto api = (RendererApi*)(RendererApi::Get().get());

		std::vector<vk::AttachmentDescription> attachmentsDescriptions;
		for (auto& att : attachments) {
			vk::AttachmentDescription desc(
				vk::AttachmentDescriptionFlags(),
				formatToVk(att.format, api->mPhysicalDevice),
				vk::SampleCountFlagBits::e1,
				att.loadop == StoreLoadOp::CLEAR ? vk::AttachmentLoadOp::eClear : vk::AttachmentLoadOp::eLoad,
				att.storeop == StoreLoadOp::CLEAR ? vk::AttachmentStoreOp::eDontCare : vk::AttachmentStoreOp::eStore,
				vk::AttachmentLoadOp::eDontCare,
				vk::AttachmentStoreOp::eDontCare,
				vk::ImageLayout::eUndefined,
				vk::ImageLayout::eColorAttachmentOptimal
			);
			attachmentsDescriptions.push_back(desc);
		}
		std::vector<vk::AttachmentReference> colorRefs;
		std::vector<vk::SubpassDescription> subpass_descs;
		subpass_descs.reserve(subpasses.size());
		for (auto& subpass : subpasses)
		{
			vk::SubpassDescription pass;
			auto prevSize = colorRefs.size();
			for (auto& color : subpass.colorAttachments)
			{
				colorRefs.push_back(vk::AttachmentReference(color, vk::ImageLayout::eColorAttachmentOptimal));
			}

			pass.colorAttachmentCount = subpass.colorAttachments.size();
			pass.pColorAttachments = &colorRefs[prevSize];

			subpass_descs.push_back(pass);
		}

		vk::RenderPassCreateInfo createInfo({}, attachmentsDescriptions, subpass_descs);

		mAttachments = attachments;
		mSubpasses = subpasses;
		mRenderPass = api->mDevice.createRenderPass(createInfo);
	}
	const std::vector<AttachmentDescription>& RenderPass::GetAttachments()
	{
		return mAttachments;
	}
	const std::vector<Subpass>& RenderPass::GetSubpasses()
	{
		return mSubpasses;
	}
}