#pragma once
#include <RoxEngine/renderer/GraphicsPipeline.h>
#include <vulkan/vulkan.hpp>
namespace RoxEngine::Vulkan
{
	class GraphicsPipeline : public RoxEngine::GraphicsPipeline
	{
	public:
		GraphicsPipeline(const BufferLayout& layout,std::shared_ptr<Material> mat, std::shared_ptr<Framebuffer> fb);

		vk::Pipeline mPipeline;
		vk::PipelineLayout mLayout;
	};
}