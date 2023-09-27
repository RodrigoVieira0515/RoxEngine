#include "VBuffers.h"
#include <Platform/Vulkan/VRendererApi.h>

namespace RoxEngine::Vulkan {
	VertexBuffer::VertexBuffer(const BufferLayout& layout, void* data, size_t size)
	{
		mLayout = layout;
		mSize = size;

		auto api = (RendererApi*)RendererApi::Get().get();

		uint32_t queues[] = {api->mQueueFamilyIndices.graphicsFamily.value(), api->mQueueFamilyIndices.presentFamily.value()};
		bool duplicate = queues[0] == queues[1];

		vk::BufferCreateInfo info({}, size, vk::BufferUsageFlagBits::eVertexBuffer, duplicate?vk::SharingMode::eExclusive:vk::SharingMode::eConcurrent,queues);
		if (duplicate)
			info.queueFamilyIndexCount = 0;

		vma::AllocationCreateInfo allocInfo(vma::AllocationCreateFlags(), vma::MemoryUsage::eCpuToGpu, vk::MemoryPropertyFlags(), vk::MemoryPropertyFlags(),0, {}, data,0.f);
		std::tie(mBuffer,mAllocation) = api->mAllocator.createBuffer(info, allocInfo);
		if (!data)
			return;

		RE_CORE_FATAL("TODO: implement VertexBuffer staging buffer");


		void* buffer_data;
		vmaMapMemory(api->mAllocator, mAllocation, &buffer_data);

		std::memcpy(buffer_data, data, size);

		vmaUnmapMemory(api->mAllocator, mAllocation);
	}
	
	const BufferLayout& VertexBuffer::GetLayout()
	{
		return mLayout;
	}
	
	IndexBuffer::IndexBuffer(uint32_t* data, size_t count)
	{
		mCount = count;
		auto api = (RendererApi*)RendererApi::Get().get();

		uint32_t queues[] = { api->mQueueFamilyIndices.graphicsFamily.value(), api->mQueueFamilyIndices.presentFamily.value() };
		bool duplicate = queues[0] == queues[1];

		vk::BufferCreateInfo info({}, count * sizeof(uint32_t), vk::BufferUsageFlagBits::eIndexBuffer, duplicate ? vk::SharingMode::eExclusive : vk::SharingMode::eConcurrent, queues);
		if (duplicate)
			info.queueFamilyIndexCount = 0;

		vma::AllocationCreateInfo allocInfo(vma::AllocationCreateFlags(), vma::MemoryUsage::eCpuToGpu, vk::MemoryPropertyFlags(), vk::MemoryPropertyFlags(), 0, {}, data, 0.f);
		std::tie(mBuffer, mAllocation) = api->mAllocator.createBuffer(info, allocInfo);

		void* buffer_data;
		vmaMapMemory(api->mAllocator, mAllocation, &buffer_data);

		std::memcpy(buffer_data, data, sizeof(uint32_t) * count);

		vmaUnmapMemory(api->mAllocator, mAllocation);
	}

	size_t IndexBuffer::GetCount()
	{
		return mCount;
	}
}
