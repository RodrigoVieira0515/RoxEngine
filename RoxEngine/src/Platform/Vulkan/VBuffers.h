#pragma once
#include <RoxEngine/renderer/Buffers.h>
#include <vulkan/vulkan.hpp>
#include <vma-hpp/vk_mem_alloc.hpp>

namespace RoxEngine::Vulkan {
	class VertexBuffer : public RoxEngine::VertexBuffer
	{
	public:
		VertexBuffer(const BufferLayout& layout, void* data, size_t size);
		const BufferLayout& GetLayout() override;
		
		BufferLayout mLayout;
		size_t mSize;
		vk::Buffer mBuffer;
		vma::Allocation mAllocation;
	};
	class IndexBuffer : public RoxEngine::IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* data, size_t count);
		size_t GetCount();
		
		size_t mCount;
		vk::Buffer mBuffer;
		vma::Allocation mAllocation;
	};
}