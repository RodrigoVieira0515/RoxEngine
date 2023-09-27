#pragma once
#include <memory>
#include <RoxEngine/renderer/Framebuffer.h>
#include <RoxEngine/renderer/GraphicsPipeline.h>
#include <RoxEngine/renderer/RenderPass.h>
#include <RoxEngine/renderer/VertexArray.h>
#include <glm/ext/vector_float4.hpp>

namespace RoxEngine {
	class CommandBuffer
	{
	public:
		static std::shared_ptr<CommandBuffer> Create();

		virtual void BeginWrite() = 0;
		virtual void BindRenderPass(std::shared_ptr<RenderPass>& renderPass, std::shared_ptr<Framebuffer>& framebuffer, glm::vec4 clearColor) = 0;
		virtual void BindGraphicsPipeline(std::shared_ptr<RoxEngine::GraphicsPipeline>& pipeline) = 0;
		virtual void BindVertexArray(std::shared_ptr<RoxEngine::VertexArray>& vb) = 0;
		virtual void Draw(uint32_t count) = 0;
		virtual void EndWrite() = 0;
		virtual void Execute() = 0;
	};
}

