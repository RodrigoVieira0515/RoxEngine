#include <Platform/Vulkan/VUniformBuffer.h>
#include <Platform/Vulkan/VRendererApi.h>

namespace RoxEngine::Vulkan
{
	UniformBuffer::UniformBuffer(UboDesc desc) : RoxEngine::UniformBuffer(desc)
	{
		auto api = (RendererApi*)RendererApi::Get().get();
		mDesc = desc;
		
		uint32_t queues[] = { api->mQueueFamilyIndices.graphicsFamily.value(), api->mQueueFamilyIndices.presentFamily.value() };
		bool duplicate = queues[0] == queues[1];

		vk::BufferCreateInfo info({}, desc.mSize, vk::BufferUsageFlagBits::eUniformBuffer, duplicate ? vk::SharingMode::eExclusive : vk::SharingMode::eConcurrent, queues);
		if (duplicate)
			info.queueFamilyIndexCount = 0;

		vma::AllocationCreateInfo allocInfo(vma::AllocationCreateFlags(), vma::MemoryUsage::eCpuToGpu, vk::MemoryPropertyFlags(), vk::MemoryPropertyFlags(), 0, {}, nullptr, 0.f);
		std::tie(mUbo, mAllocation) = api->mAllocator.createBuffer(info, allocInfo);
		RE_CORE_ERROR("Todo: Uniform buffer staging buffer");
	}
	UniformBuffer::~UniformBuffer()
	{

	}
}