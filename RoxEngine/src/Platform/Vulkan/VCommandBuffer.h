#pragma once
#include <RoxEngine/renderer/CommandBuffer.h>
#include <vulkan/vulkan.hpp>

namespace RoxEngine::Vulkan {
	class Framebuffer;
	class CommandBuffer : public RoxEngine::CommandBuffer
	{
	public:
		CommandBuffer();
		/*
		void BeginWrite() override;
		void BindRenderPass(std::shared_ptr<RoxEngine::RenderPass>& renderPass, std::shared_ptr<RoxEngine::Framebuffer>& framebuffer, glm::vec4 clearColor) override;
		void BindGraphicsPipeline(std::shared_ptr<RoxEngine::GraphicsPipeline>& pipeline) override;
		void BindVertexArray(std::shared_ptr<RoxEngine::VertexArray>& va) override;
		void Draw(uint32_t count) override;
		void EndWrite() override;*/
		void Execute() override;

		vk::CommandBuffer mBuffer;
		std::vector<std::shared_ptr<RoxEngine::Material>> materials;
		std::shared_ptr<Framebuffer> mFramebuffer;
		std::shared_ptr<RoxEngine::GraphicsPipeline> mPipeline;
	};
}

