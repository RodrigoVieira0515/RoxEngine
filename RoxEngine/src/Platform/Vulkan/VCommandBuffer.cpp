#include "VCommandBuffer.h"
#include <Platform/Vulkan/VRendererApi.h>
#include <Platform/Vulkan/VRenderPass.h>
#include <Platform/Vulkan/VFramebuffer.h>
#include <Platform/Vulkan/VGraphicsPipeline.h>
#include <Platform/Vulkan/VBuffers.h>
#include <Platform/Vulkan/VMaterial.h>

namespace RoxEngine::Vulkan {
	CommandBuffer::CommandBuffer()
	{
		auto api = (RendererApi*)RendererApi::Get().get();
		vk::CommandBufferAllocateInfo info(api->mCommandPool, vk::CommandBufferLevel::ePrimary, 1);
		mBuffer = api->mDevice.allocateCommandBuffers(info).front();
	}
	void CommandBuffer::BeginWrite()
	{
		vk::CommandBufferBeginInfo info(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		mBuffer.begin(info);
	}

	void CommandBuffer::BindRenderPass(std::shared_ptr<RoxEngine::RenderPass>& renderPass, std::shared_ptr<RoxEngine::Framebuffer>& framebuffer, glm::vec4 clearColor)
	{
		auto rawPass = ((RenderPass*)renderPass.get())->mRenderPass;
		auto rawFb = ((Framebuffer*)framebuffer.get())->mFramebuffer;
		auto VulkanClearColor = vk::ClearValue(vk::ClearColorValue({ clearColor.x,clearColor.y,clearColor.z,clearColor.w }));
		auto size = framebuffer->GetSize();
		mBuffer.beginRenderPass(vk::RenderPassBeginInfo(rawPass, rawFb, vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D(size.x,size.y)), VulkanClearColor), vk::SubpassContents::eInline);

		mFramebuffer = std::dynamic_pointer_cast<Framebuffer>(framebuffer);
	}
	void CommandBuffer::BindGraphicsPipeline(std::shared_ptr<RoxEngine::GraphicsPipeline>& pipeline)
	{
		mPipeline = pipeline;
		auto vPipeline = ((GraphicsPipeline*)pipeline.get());
		
		mBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vPipeline->mPipeline);
		if (vPipeline->mMat->GetShader()->GetNumOfUbos() > 0)
		{
			auto api = (RendererApi*)RendererApi::Get().get();
			auto mat = vPipeline->mMat;
			auto vmat = (Material*)mat.get();
			for (auto& ubo : vmat->mUbos) {
				auto vubo = (UniformBuffer*)ubo.second.get();
				if (vubo->mChanged) {
					void* buffer_data;
					vmaMapMemory(api->mAllocator, vubo->mAllocation, &buffer_data);

					std::memcpy(buffer_data, vubo->mData, vubo->mDesc.mSize);

					vmaUnmapMemory(api->mAllocator, vubo->mAllocation);
				}
			}
			std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
			for (auto& ubo : vmat->mUbos) {
				auto vubo = (UniformBuffer*)ubo.second.get();
				if (vubo->mChanged) {
					auto bufferInfo = vk::DescriptorBufferInfo(vubo->mUbo, 0, vubo->mDesc.mSize);
					writeDescriptorSets.push_back(vk::WriteDescriptorSet(vPipeline->mDescriptorSet, vubo->mDesc.mBinding, 0, vk::DescriptorType::eUniformBuffer, nullptr, bufferInfo));
				}
			}
			if (writeDescriptorSets.size() > 0)
				api->mDevice.updateDescriptorSets(writeDescriptorSets, nullptr);
			api->mDevice.waitIdle();
			mBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, vPipeline->mLayout, 0, vPipeline->mDescriptorSet, nullptr);
		}
		materials.push_back(vPipeline->mMat);
	}

	void CommandBuffer::BindVertexArray(std::shared_ptr<RoxEngine::VertexArray>& va)
	{
		for (auto& vb : va->GetVertexBuffers()) {
			auto rawVb = ((VertexBuffer*)vb.get())->mBuffer;
			mBuffer.bindVertexBuffers(0, rawVb, { 0 });
		}
		auto rawIb = ((IndexBuffer*)va->GetIndexBuffer().get())->mBuffer;
		mBuffer.bindIndexBuffer(rawIb, 0, vk::IndexType::eUint32);
	}

	void CommandBuffer::Draw(uint32_t count)
	{
		mBuffer.setViewport(0, vk::Viewport(0, 0, 800, 800, 0, 1));
		mBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D(800, 800)));
		mBuffer.drawIndexed(count,1,0,0,0);
	}
	
	void CommandBuffer::EndWrite()
	{
		mBuffer.endRenderPass();
		mBuffer.end();
		auto api = (RendererApi*)RendererApi::Get().get();
		api->mExecuteCmd = this;
	}
	
	void CommandBuffer::Execute()
	{
		auto api = (RendererApi*)RendererApi::Get().get();

		api->mDevice.waitIdle();
		api->mExecuteCmd = this;
		vk::SubmitInfo submit_info(nullptr, nullptr, mBuffer,nullptr);
		api->mGraphicsQueue.submit(submit_info, nullptr);
		api->mDevice.waitIdle();
	}
}
