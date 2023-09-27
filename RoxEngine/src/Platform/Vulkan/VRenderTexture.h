#pragma once
#include <RoxEngine/renderer/RenderTexture.h>
#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>
#include <vma-hpp/vk_mem_alloc.hpp>

namespace RoxEngine::Vulkan {
	vk::Format formatToVk(const FramebufferTexFormat& f, vk::PhysicalDevice& device);
	FramebufferTexFormat vkToFormat(vk::Format format);

	class RenderTexture : public RoxEngine::RenderTexture
	{
	public:
		RenderTexture(uint32_t width, uint32_t height,FramebufferTexFormat colorFormat);
		~RenderTexture();

		FramebufferTexFormat mFormat;
		FramebufferTexFormat mInternalFormat;
		vk::Image mImage;
		vma::Allocation mAllocation;
	};
}

