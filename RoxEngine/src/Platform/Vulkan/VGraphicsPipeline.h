#pragma once
#include <RoxEngine/renderer/GraphicsPipeline.h>
#include <vulkan/vulkan.hpp>
namespace RoxEngine::Vulkan
{
	class GraphicsPipeline : public RoxEngine::GraphicsPipeline
	{
	public:
		GraphicsPipeline(const BufferLayout& layout,std::shared_ptr<RoxEngine::Material> mat, std::shared_ptr<Framebuffer> fb);

		std::shared_ptr<RoxEngine::Material> mMat;
		vk::Pipeline mPipeline;
		vk::PipelineLayout mLayout;
		vk::DescriptorSetLayout mSetLayout = nullptr;
		vk::DescriptorPool mDescriptorPool = nullptr;
		vk::DescriptorSet mDescriptorSet = nullptr;
	};
}