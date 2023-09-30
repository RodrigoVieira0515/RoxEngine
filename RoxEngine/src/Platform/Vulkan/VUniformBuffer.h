#pragma once
#include <RoxEngine/renderer/UniformBuffer.h>
#include <vma-hpp/vk_mem_alloc.hpp>

namespace RoxEngine::Vulkan
{
	class UniformBuffer : public RoxEngine::UniformBuffer
	{
	public:
		UniformBuffer(UboDesc desc);
		virtual ~UniformBuffer();

		bool HasChanged() { return mChanged; }

		vk::Buffer mUbo;
		vma::Allocation mAllocation;
	};
}