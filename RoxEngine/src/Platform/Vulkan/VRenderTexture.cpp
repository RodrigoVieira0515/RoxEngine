#include "VRenderTexture.h"
#include <RoxEngine/core/Assert.h>
#include <RoxEngine/renderer/RendererApi.h>
#include "VRenderTexture.h"
#include <vulkan/vulkan.hpp>
#include <platform/Vulkan/VRendererApi.h>
#include <RoxEngine/core/Assert.h>

#include <vulkan/vulkan.h>

namespace RoxEngine::Vulkan {
	vk::Format formatToVk(const FramebufferTexFormat& f, vk::PhysicalDevice& device) {
		static std::unordered_map<FramebufferTexFormat, vk::Format> formatToVk;
		auto it = formatToVk.find(f);
		if (it != formatToVk.end())
			return it->second;

		vk::Format preFormat;
		
		switch (f)
		{
		case FramebufferTexFormat::R8:
			preFormat = vk::Format::eR8Unorm; break;
		case FramebufferTexFormat::RG8:
			preFormat = vk::Format::eR8G8Unorm; break;
		case FramebufferTexFormat::RGB8:
			preFormat = vk::Format::eR8G8B8Unorm; break;
		case FramebufferTexFormat::RGBA8:
			preFormat = vk::Format::eR8G8B8A8Unorm; break;
		default:
			assert(false);
		}
		auto formatProps = device.getFormatProperties(preFormat);
		bool linearTilingSupported = (formatProps.linearTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment) && (formatProps.linearTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrc) && (formatProps.linearTilingFeatures & vk::FormatFeatureFlagBits::eTransferDst);
		bool optimalTilingSupported = (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment) && (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrc) && (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferDst);
		bool supported = linearTilingSupported || optimalTilingSupported;
		if (supported) {
			formatToVk.insert({ f,preFormat });
			return preFormat;
		}
		vk::Format afterFormat = vk::Format::eUndefined;

		switch (f)
		{
		case FramebufferTexFormat::R8:
			afterFormat = vk::Format::eR8G8Unorm; break;
		case FramebufferTexFormat::RG8:
			afterFormat = vk::Format::eR8G8B8Unorm; break;
		case FramebufferTexFormat::RGB8:
			afterFormat = vk::Format::eR8G8B8A8Unorm; break;
		case FramebufferTexFormat::RGBA8:
			//TODO: maybe increese precision?
			RE_CORE_ASSERT(false, "Format is not supported, {}", "");
			break;
		}
		formatProps = device.getFormatProperties(afterFormat);
		
		linearTilingSupported = (formatProps.linearTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment) && (formatProps.linearTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrc) && (formatProps.linearTilingFeatures & vk::FormatFeatureFlagBits::eTransferDst);
		optimalTilingSupported = (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment) && (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrc) && (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferDst);
		supported = linearTilingSupported || optimalTilingSupported;

		RE_CORE_ASSERT(supported, "Format is not supported");
		formatToVk.insert({ f,afterFormat });
		return afterFormat;
	}
	FramebufferTexFormat vkToFormat(vk::Format format)
	{
		switch (format)
		{
		case vk::Format::eR8Unorm: return FramebufferTexFormat::R8; break;
		case vk::Format::eR8G8Unorm: return FramebufferTexFormat::RG8; break;
		case vk::Format::eR8G8B8Unorm: return FramebufferTexFormat::RGB8; break;
		case vk::Format::eR8G8B8A8Unorm: return FramebufferTexFormat::RGBA8; break;
		default:
			assert(false);
		}
	}
	RenderTexture::RenderTexture(uint32_t width, uint32_t height, FramebufferTexFormat colorFormat)
	{
		mFormat = colorFormat;
		auto api = (RendererApi*)(RendererApi::Get().get());

		auto internalFormat = formatToVk(colorFormat, api->mPhysicalDevice);

		mFormat = colorFormat;
		mInternalFormat = vkToFormat(internalFormat);
		auto formatProps = api->mPhysicalDevice.getFormatProperties(internalFormat);
		auto tilingOption = formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment ? vk::ImageTiling::eOptimal : vk::ImageTiling::eLinear;

		vk::ImageCreateInfo info({},
			vk::ImageType::e2D, 
			internalFormat,
			vk::Extent3D(width, height, 1), 
			1, 1,
			vk::SampleCountFlagBits::e1, 
			tilingOption, 
			vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst,
			vk::SharingMode::eExclusive);
		info.setInitialLayout(vk::ImageLayout::eUndefined);
		
		vma::AllocationCreateInfo allocInfo;
		
		std::tie(mImage, mAllocation) = api->mAllocator.createImage(info, allocInfo);

		{
			vk::CommandBufferAllocateInfo cmdAllocInfo(api->mCommandPool, vk::CommandBufferLevel::ePrimary, 1);
			auto cmd = api->mDevice.allocateCommandBuffers(cmdAllocInfo).front();
			cmd.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
			
			vk::ImageMemoryBarrier barrierSrcToTransfer(
				vk::AccessFlags(),
				vk::AccessFlagBits::eColorAttachmentWrite,
				vk::ImageLayout::eUndefined,
				vk::ImageLayout::eColorAttachmentOptimal,
				VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
				mImage,
				vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

			cmd.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::DependencyFlags(), nullptr, nullptr, barrierSrcToTransfer);

			cmd.end();

			vk::SubmitInfo submit_info(nullptr, nullptr, cmd, nullptr);
			api->mGraphicsQueue.submit(submit_info, nullptr);
			api->mDevice.waitIdle();
		}
	}
	RenderTexture::~RenderTexture()
	{
		auto api = (RendererApi*)(RendererApi::Get().get());

		api->mDevice.destroy(mImage);
	}
}
