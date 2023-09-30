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
	void CommandBuffer::BindRenderPass(std::shared_ptr<RenderPass>& renderPass, std::shared_ptr<Framebuffer>& framebuffer, glm::vec4 clearColor)
	{
		Operation op = { mOperations.size(), Operation::BIND_RENDER_PASS, Operation::opBindRp{renderPass, framebuffer, clearColor} };
		mOperations.emplace_back(op);
	}
	void CommandBuffer::BindGraphicsPipeline(std::shared_ptr<RoxEngine::GraphicsPipeline>& pipeline)
	{
		Operation op = { mOperations.size(), Operation::BIND_GRAPHICS_PIPELINE, Operation::opBindGp{pipeline} };
		mOperations.emplace_back(op);
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
	}
	void CommandBuffer::CallCmd(std::shared_ptr<CommandBuffer> cmd)
	{
		Operation op = { mOperations.size(), Operation::CALL_CMD, Operation::opCICmd{cmd} };
		mOperations.emplace_back(op);
	}
	void CommandBuffer::InlineCmd(std::shared_ptr<CommandBuffer> cmd)
	{
		Operation op = { mOperations.size(), Operation::INLINE_CMD, Operation::opCICmd{cmd} };
		mOperations.emplace_back(op);
	}
}
