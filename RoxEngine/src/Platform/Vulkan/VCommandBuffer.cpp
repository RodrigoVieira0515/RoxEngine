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
		
	}

	void CommandBuffer::Execute()
	{
		auto api = (RendererApi*)RendererApi::Get().get();
		vk::CommandBufferAllocateInfo info(api->mCommandPool, vk::CommandBufferLevel::ePrimary, 1);
		mBuffer = api->mDevice.allocateCommandBuffers(info).front();
		
		vk::CommandBufferBeginInfo cmdBeginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		mBuffer.begin(cmdBeginInfo);

		//b for currentlly bound
		std::shared_ptr<RenderPass> bRenderPass = nullptr;
		std::shared_ptr<Framebuffer> bFramebuffer = nullptr;
		std::shared_ptr<GraphicsPipeline> bGraphicsPipeline = nullptr;
		std::shared_ptr<Material> bMaterial = nullptr;
		std::shared_ptr<VertexArray> bVertexArray = nullptr;
		for (auto& op : mOperations)
		{
			switch (op.type)
			{
				case Operation::BIND_RENDER_PASS: {
					auto& data = std::get<Operation::opBindRp>(op.data);
					auto rawPass = ((RenderPass*)data.rp.get())->mRenderPass;
					auto rawFb = ((Framebuffer*)data.fb.get())->mFramebuffer;
					auto VulkanClearColor = vk::ClearValue(vk::ClearColorValue({ data.clearColor.x,data.clearColor.y,data.clearColor.z,data.clearColor.w }));
					auto size = data.fb->GetSize();
					mBuffer.beginRenderPass(vk::RenderPassBeginInfo(rawPass, rawFb, vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D(size.x, size.y)), VulkanClearColor), vk::SubpassContents::eInline);

					bFramebuffer = std::dynamic_pointer_cast<Framebuffer>(data.fb);
					bRenderPass = std::dynamic_pointer_cast<RenderPass>(data.rp);
					break;
				}
				case Operation::BIND_GRAPHICS_PIPELINE: {
					auto& data = std::get<Operation::opBindGp>(op.data);
					auto vPipeline = std::static_pointer_cast<GraphicsPipeline>(data.gp);
					mBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vPipeline->mPipeline);

					bGraphicsPipeline = vPipeline;
					bMaterial = std::dynamic_pointer_cast<Material>(vPipeline->mMat);

					RE_CORE_FATAL("TODO: ADD A PROPER WAY TO UPDATE DESCRIPTOR AKA UBOs");
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
					break;
				}
				case Operation::BIND_VERTEX_ARRAY: {
					auto& data = std::get<Operation::opBindVa>(op.data);
					auto va = std::dynamic_pointer_cast<VertexArray>(data.va);
					for (auto& vb : va->GetVertexBuffers()) {
						auto rawVb = ((VertexBuffer*)vb.get())->mBuffer;
						mBuffer.bindVertexBuffers(0, rawVb, { 0 });
					}
					auto rawIb = ((IndexBuffer*)va->GetIndexBuffer().get())->mBuffer;
					mBuffer.bindIndexBuffer(rawIb, 0, vk::IndexType::eUint32);

					bVertexArray = va;
					break;
				}
				case Operation::CALL_CMD:
					RE_CORE_ASSERT(false, "CommandBuffer Operation CallCmd not implemented")
					break;
				case Operation::INLINE_CMD:
					RE_CORE_ASSERT(false, "CommandBuffer Operation InlineCmd not implemented")
					break;
				case Operation::DRAW: {
					auto& data = std::get<Operation::opDraw>(op.data);

					auto size = bFramebuffer->GetSize();

					mBuffer.setViewport(0, vk::Viewport(0, 0, size.x,size.y, 0, 1));
					mBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D(size.x, size.y)));
					mBuffer.drawIndexed(data.count, 1, 0, 0, 0);
					break;
				}
			}
		}
		if (bFramebuffer != nullptr) {
			mBuffer.endRenderPass();
		}
		mBuffer.end();

		vk::SubmitInfo submit_info(nullptr, nullptr, mBuffer, nullptr);
		api->mGraphicsQueue.submit(submit_info, nullptr);
		api->mDevice.waitIdle();
	}
}
