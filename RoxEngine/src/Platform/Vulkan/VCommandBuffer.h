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

		struct CurrentBound
		{
			std::shared_ptr<RenderPass> bRenderPass = nullptr;
			std::shared_ptr<Framebuffer> bFramebuffer = nullptr;
			std::shared_ptr<GraphicsPipeline> bGraphicsPipeline = nullptr;
			std::shared_ptr<Material> bMaterial = nullptr;
			std::shared_ptr<VertexArray> bVertexArray = nullptr;
		};
		void ParseOperation(Operation& op, CurrentBound& cb);
		void CreateCache() override;
		void Execute() override;

		vk::CommandBuffer mPrimaryBuffer;

		std::vector<std::shared_ptr<RoxEngine::Material>> materials;
		std::shared_ptr<Framebuffer> mFramebuffer;
		std::shared_ptr<RoxEngine::GraphicsPipeline> mPipeline;
	};
}

