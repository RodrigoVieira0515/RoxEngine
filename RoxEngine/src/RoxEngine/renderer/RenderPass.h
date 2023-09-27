#pragma once
#include <memory>
#include <vector>
#include "RenderTexture.h"
namespace RoxEngine {
	enum class StoreLoadOp {
		CLEAR,
		LOAD
	};
	struct AttachmentDescription
	{
		FramebufferTexFormat format;
		StoreLoadOp loadop;
		StoreLoadOp storeop;
	};
	struct Subpass
	{
		Subpass(uint32_t dependency, const std::vector<uint32_t>& inputAttachments, const std::vector<uint32_t>& colorAttachments, const std::vector<uint32_t>& preserveAttachment);
		// 0 for nothing and everything above is the index to the subpasses ex: 1 = index number 0
		uint32_t dependency = 0;
		// read-only attachments prevents from clearing
		std::vector<uint32_t> inputAttachments;
		// attachments that will be drawn to
		std::vector<uint32_t> colorAttachments;
		// only prevents from clearing
		std::vector<uint32_t> preserveAttachments;
	};
	class RenderPass
	{
	public:
		static std::shared_ptr<RenderPass> Create(const std::vector<AttachmentDescription>& attachments, const std::vector<Subpass>& subpasses);

		virtual const std::vector<AttachmentDescription>& GetAttachments() = 0;
		virtual const std::vector<Subpass>& GetSubpasses() = 0;
	};
}

