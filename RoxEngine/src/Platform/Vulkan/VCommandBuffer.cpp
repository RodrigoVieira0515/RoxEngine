#include "VCommandBuffer.h"
#include <Platform/Vulkan/VRendererApi.h>
#include <Platform/Vulkan/VRenderPass.h>
#include <Platform/Vulkan/VFramebuffer.h>
#include <Platform/Vulkan/VGraphicsPipeline.h>
#include <Platform/Vulkan/VBuffers.h>
#include <Platform/Vulkan/VMaterial.h>
#include <Platform/Vulkan/VRenderTexture.h>
#include <deque>

namespace RoxEngine::Vulkan {
	CommandBuffer::CommandBuffer()
	{
		
	}

	void CommandBuffer::ParseOperation(Operation& op, CurrentBound& currentBound)
	{
		switch (op.type)
		{
		case Operation::BIND_RENDER_PASS: {
			auto& data = std::get<Operation::opBindRp>(op.data);
			auto rawPass = ((RenderPass*)data.rp.get())->mRenderPass;
			auto rawFb = ((Framebuffer*)data.fb.get())->mFramebuffer;
			auto VulkanClearColor = vk::ClearValue(vk::ClearColorValue({ data.clearColor.x,data.clearColor.y,data.clearColor.z,data.clearColor.w }));
			auto size = data.fb->GetSize();
			mPrimaryBuffer.beginRenderPass(vk::RenderPassBeginInfo(rawPass, rawFb, vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D(size.x, size.y)), VulkanClearColor), vk::SubpassContents::eInline);

			currentBound.bFramebuffer = std::dynamic_pointer_cast<Framebuffer>(data.fb);
			currentBound.bRenderPass = std::dynamic_pointer_cast<RenderPass>(data.rp);
			break;
		}
		case Operation::BIND_GRAPHICS_PIPELINE: {
			auto& data = std::get<Operation::opBindGp>(op.data);
			auto vPipeline = std::static_pointer_cast<GraphicsPipeline>(data.gp);
			mPrimaryBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vPipeline->mPipeline);

			currentBound.bGraphicsPipeline = vPipeline;
			currentBound.bMaterial = std::dynamic_pointer_cast<Material>(vPipeline->mMat);

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
				mPrimaryBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, vPipeline->mLayout, 0, vPipeline->mDescriptorSet, nullptr);
			}
			break;
		}
		case Operation::BIND_VERTEX_ARRAY: {
			auto& data = std::get<Operation::opBindVa>(op.data);
			auto va = std::dynamic_pointer_cast<VertexArray>(data.va);
			for (auto& vb : va->GetVertexBuffers()) {
				auto rawVb = ((VertexBuffer*)vb.get())->mBuffer;
				mPrimaryBuffer.bindVertexBuffers(0, rawVb, { 0 });
			}
			auto rawIb = ((IndexBuffer*)va->GetIndexBuffer().get())->mBuffer;
			mPrimaryBuffer.bindIndexBuffer(rawIb, 0, vk::IndexType::eUint32);

			currentBound.bVertexArray = va;
			break;
		}
		case Operation::DRAW: {
			auto& data = std::get<Operation::opDraw>(op.data);

			auto size = currentBound.bFramebuffer->GetSize();

			mPrimaryBuffer.setViewport(0, vk::Viewport(0, 0, size.x, size.y, 0, 1));
			mPrimaryBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D(size.x, size.y)));
			mPrimaryBuffer.drawIndexed(data.count, 1, 0, 0, 0);
			break;
		}
		case Operation::BLIT_FB: {
			auto& data = std::get<Operation::opBlitFb>(op.data);

			auto src = std::static_pointer_cast<Framebuffer>(data.src);
			auto dst = std::static_pointer_cast<Framebuffer>(data.dst);

			auto srcSize = data.src->GetSize();
			auto dstSize = data.dst->GetSize();

			vk::ImageSubresourceLayers subresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
			std::array<vk::Offset3D, 2> srcOffsets = { vk::Offset3D(0, 0, 0),vk::Offset3D(srcSize.x, srcSize.y, 1) };
			std::array<vk::Offset3D, 2> dstOffsets = { vk::Offset3D(0, 0, 0),vk::Offset3D(dstSize.x, dstSize.y, 1) };

			vk::ImageBlit region(subresourceLayers, srcOffsets, subresourceLayers, dstOffsets);

			auto subresourceRange = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

			auto srcTex = src->mSwapchain ?
				src->mImg
				:
				std::static_pointer_cast<RenderTexture>(src->mColorAttachments[0])->mImage;
			auto dstTex = dst->mSwapchain ?
				dst->mImg
				:
				std::static_pointer_cast<RenderTexture>(dst->mColorAttachments[0])->mImage;


			// convert to transfer read/write
			std::array<vk::ImageMemoryBarrier, 2> toCopy = {
				vk::ImageMemoryBarrier(
					vk::AccessFlagBits::eColorAttachmentWrite,
					vk::AccessFlagBits::eTransferRead,
					vk::ImageLayout::eColorAttachmentOptimal,
					vk::ImageLayout::eTransferSrcOptimal,
					VK_QUEUE_FAMILY_IGNORED,
					VK_QUEUE_FAMILY_IGNORED,
					srcTex,
					subresourceRange
				),
				vk::ImageMemoryBarrier(
					vk::AccessFlagBits::eNone,
					vk::AccessFlagBits::eTransferWrite,
					vk::ImageLayout::eUndefined,
					vk::ImageLayout::eTransferDstOptimal,
					VK_QUEUE_FAMILY_IGNORED,
					VK_QUEUE_FAMILY_IGNORED,
					dstTex,
					subresourceRange
				),
			};
			std::array<vk::ImageMemoryBarrier, 2> fromCopy = {
			vk::ImageMemoryBarrier(
				vk::AccessFlagBits::eTransferRead,
				vk::AccessFlagBits::eColorAttachmentWrite,
				vk::ImageLayout::eTransferSrcOptimal,
				vk::ImageLayout::eColorAttachmentOptimal,
				VK_QUEUE_FAMILY_IGNORED,
				VK_QUEUE_FAMILY_IGNORED,
				srcTex,
				subresourceRange
			),
			vk::ImageMemoryBarrier(
				vk::AccessFlagBits::eTransferWrite,
				vk::AccessFlagBits::eNone,
				vk::ImageLayout::eTransferDstOptimal,
				vk::ImageLayout::ePresentSrcKHR,
				VK_QUEUE_FAMILY_IGNORED,
				VK_QUEUE_FAMILY_IGNORED,
				dstTex,
				subresourceRange
			)
			};

			mPrimaryBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eAllCommands, vk::PipelineStageFlagBits::eTransfer, vk::DependencyFlags(), nullptr, nullptr, toCopy);
			mPrimaryBuffer.blitImage(srcTex, vk::ImageLayout::eTransferSrcOptimal, dstTex, vk::ImageLayout::eTransferDstOptimal, region, vk::Filter::eNearest);
			mPrimaryBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::DependencyFlags(), nullptr, nullptr, fromCopy);

			break;
		}
		case Operation::RAW_CALL: {
			std::get<Operation::opRawCall>(op.data).fn(this, &mPrimaryBuffer);
			break;
		}
		default: RE_CORE_ASSERT(false, "");
		}
	}

	void CommandBuffer::CreateCache()
	{
		auto api = (RendererApi*)RendererApi::Get().get();
		vk::CommandBufferAllocateInfo info(api->mCommandPool, vk::CommandBufferLevel::ePrimary, 1);
		if (!mPrimaryBuffer) {
			mPrimaryBuffer = api->mDevice.allocateCommandBuffers(info).front();
		}
		else {
			mPrimaryBuffer.reset();
		}
		vk::CommandBufferBeginInfo cmdBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		mPrimaryBuffer.begin(cmdBeginInfo);

		//b for currentlly bound

		CurrentBound currentBound;

		for (auto& op : mOperations)
		{
			switch (op.type)
			{
			case Operation::CALL_CMD:
				RE_CORE_ASSERT(false, "CommandBuffer Operation CallCmd not implemented")
					break;
			case Operation::INLINE_CMD: {
				auto& data = std::get<Operation::opCICmd>(op.data);

				std::deque<CommandBuffer*> deque;

				deque.push_back((CommandBuffer*)data.cmd.get());

				while (!deque.empty())
				{
					auto cmd = deque.back(); deque.pop_back();
					for (auto& op2 : cmd->mOperations)
					{

						switch (op2.type)
						{
						case Operation::CALL_CMD:
							RE_CORE_ASSERT(false, "CommandBuffer Operation CallCmd not implemented")
								break;
						case Operation::INLINE_CMD:
							deque.push_back((CommandBuffer*)std::get<Operation::opCICmd>(op.data).cmd.get());
							break;
						default:
							ParseOperation(op2, currentBound);
							break;
						}
					}
				}

				break;
			}
			default:
				ParseOperation(op, currentBound);
				break;
			}
		}
		if (currentBound.bFramebuffer != nullptr) {
			mPrimaryBuffer.endRenderPass();
		}
		mPrimaryBuffer.end();
	}

	void CommandBuffer::Execute()
	{
		CreateCache();
		auto api = (RendererApi*)RendererApi::Get().get();
		vk::SubmitInfo submit_info(nullptr, nullptr, mPrimaryBuffer, nullptr);
		api->mGraphicsQueue.submit(submit_info, nullptr);
	}
}
