#include "CommandBuffer.h"
#include <RoxEngine/renderer/RendererApi.h>
#include <RoxEngine/core/Assert.h>
#include <Platform/Vulkan/VCommandBuffer.h>

namespace RoxEngine {
	std::shared_ptr<CommandBuffer> CommandBuffer::Create()
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::CommandBuffer>();
		default:
			RE_CORE_ASSERT(false, fmt::format("CommandBuffer::Create() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
		return nullptr;
	}
	void CommandBuffer::Reset()
	{
		mChanged = true;
		mOperations.clear();
	}
	void CommandBuffer::BindRenderPass(std::shared_ptr<RenderPass>& renderPass, std::shared_ptr<Framebuffer>& framebuffer)
	{
		Operation op = { mOperations.size(), Operation::BIND_RENDER_PASS, Operation::opBindRp{renderPass, framebuffer} };
		mOperations.emplace_back(op);
		mChanged = true;
	}
	void CommandBuffer::BindGraphicsPipeline(std::shared_ptr<RoxEngine::GraphicsPipeline>& pipeline)
	{
		Operation op = { mOperations.size(), Operation::BIND_GRAPHICS_PIPELINE, Operation::opBindGp{pipeline} };
		mOperations.emplace_back(op);
		mChanged = true;
	}
	void CommandBuffer::BindVertexArray(std::shared_ptr<RoxEngine::VertexArray>& va)
	{
		Operation op = { mOperations.size(), Operation::BIND_VERTEX_ARRAY, Operation::opBindVa{va} };
		mOperations.emplace_back(op);
	
	}
	void CommandBuffer::Draw(uint32_t count)
	{
		Operation op = { mOperations.size(), Operation::DRAW, Operation::opDraw{count} };
		mOperations.emplace_back(op);
		mChanged = true;
	}
	void CommandBuffer::BlitFramebuffers(std::shared_ptr<RoxEngine::Framebuffer> src, std::shared_ptr<RoxEngine::Framebuffer> dst)
	{
		Operation op = { mOperations.size(), Operation::BLIT_FB, Operation::opBlitFb{src,dst} };
		mOperations.emplace_back(op);
		mChanged = true;
	}
	void CommandBuffer::CallCmd(std::shared_ptr<CommandBuffer> cmd)
	{
		Operation op = { mOperations.size(), Operation::CALL_CMD, Operation::opCICmd{cmd} };
		mOperations.emplace_back(op);
		mChanged = true;
	}
	void CommandBuffer::InlineCmd(std::shared_ptr<CommandBuffer> cmd)
	{
		Operation op = { mOperations.size(), Operation::INLINE_CMD, Operation::opCICmd{cmd} };
		mOperations.emplace_back(op);
		//mOperations.insert(mOperations.end(), cmd->mOperations.begin(), cmd->mOperations.end());
		mChanged = true;
	}
	void CommandBuffer::RawCall(std::function<void(CommandBuffer*, void*)> fn)
	{
		Operation op = { mOperations.size(), Operation::RAW_CALL, Operation::opRawCall{fn} };
		mOperations.emplace_back(op);
		mChanged = true;
	}
	void CommandBuffer::UnbindRenderPass()
	{
		Operation op = { mOperations.size(), Operation::UNBIND_RENDER_PASS, 1 };
		mOperations.emplace_back(op);
		mChanged = true;
	}
	void CommandBuffer::UnbindGraphicsPipeline()
	{
		Operation op = { mOperations.size(), Operation::UNBIND_GRAPHICS_PIPELINE, 1 };
		mOperations.emplace_back(op);
		mChanged = true;
	}
	void CommandBuffer::UnbindVertexArray()
	{
		Operation op = { mOperations.size(), Operation::UNBIND_VERTEX_ARRAY, 1};
		mOperations.emplace_back(op);
		mChanged = true;
	}
	bool CommandBuffer::HasChanged()
	{
		return mChanged;
	}
}
