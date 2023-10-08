#include "VRenderTexture.h"
#include <RoxEngine/core/Assert.h>
#include <RoxEngine/renderer/RendererApi.h>
#include "VRenderTexture.h"
#include <vulkan/vulkan.hpp>
#include <platform/Vulkan/VRendererApi.h>
#include <RoxEngine/core/Assert.h>

#include <vulkan/vulkan.h>

namespace RoxEngine::Vulkan {
	std::tuple<vk::Format,bool, bool> formatToVk(const FramebufferTexFormat& f, vk::PhysicalDevice& device) {
		static std::unordered_map<FramebufferTexFormat, std::tuple<vk::Format,bool,bool>> formatToVk;
		auto it = formatToVk.find(f);
		if (it != formatToVk.end())
			return it->second;
		bool isDepth = false;
		vk::Format preFormat;
		vk::FormatFeatureFlags formatFeatures;

		switch (f)
		{
		case FramebufferTexFormat::R8:
			preFormat = vk::Format::eR8Unorm; formatFeatures = vk::FormatFeatureFlagBits::eColorAttachment; break;
		case FramebufferTexFormat::RG8:
			preFormat = vk::Format::eR8G8Unorm; formatFeatures = vk::FormatFeatureFlagBits::eColorAttachment; break;
		case FramebufferTexFormat::RGB8:
			preFormat = vk::Format::eR8G8B8Unorm; formatFeatures = vk::FormatFeatureFlagBits::eColorAttachment; break;
		case FramebufferTexFormat::RGBA8:
			preFormat = vk::Format::eR8G8B8A8Unorm; formatFeatures = vk::FormatFeatureFlagBits::eColorAttachment; break;

		case FramebufferTexFormat::D32F:
			isDepth = true;
			preFormat = vk::Format::eD32Sfloat; formatFeatures = vk::FormatFeatureFlagBits::eDepthStencilAttachment; break;
		case FramebufferTexFormat::D32FS8U:
			isDepth = true;
			preFormat = vk::Format::eD32SfloatS8Uint; formatFeatures = vk::FormatFeatureFlagBits::eDepthStencilAttachment; break;
		case FramebufferTexFormat::D24UNS8U:
			isDepth = true;
			preFormat = vk::Format::eD32SfloatS8Uint; formatFeatures = vk::FormatFeatureFlagBits::eDepthStencilAttachment; break;
		default:
			assert(false);
		}
		auto formatProps = device.getFormatProperties(preFormat);
		bool linearTilingSupported = (formatProps.linearTilingFeatures & formatFeatures) && (formatProps.linearTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrc) && (formatProps.linearTilingFeatures & vk::FormatFeatureFlagBits::eTransferDst);
		bool optimalTilingSupported = (formatProps.optimalTilingFeatures & formatFeatures) && (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrc) && (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferDst);
		bool supported = linearTilingSupported || optimalTilingSupported;
		if (supported) {
			formatToVk.insert({ f,{ preFormat, linearTilingSupported, isDepth} });
			return { preFormat, linearTilingSupported, isDepth};
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
		default:
			RE_CORE_ASSERT(false, "Format is not supported, RGBA8");
			break;
		}
		formatProps = device.getFormatProperties(afterFormat);
		
		linearTilingSupported = (formatProps.linearTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment) && (formatProps.linearTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrc) && (formatProps.linearTilingFeatures & vk::FormatFeatureFlagBits::eTransferDst);
		optimalTilingSupported = (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eColorAttachment) && (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrc) && (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferDst);
		supported = linearTilingSupported || optimalTilingSupported;

		RE_CORE_ASSERT(supported, "Format is not supported");
		formatToVk.insert({ f,{ afterFormat, linearTilingSupported, isDepth} });
		return { afterFormat,linearTilingSupported, isDepth};
	}
	FramebufferTexFormat vkToFormat(vk::Format format)
	{
		switch (format)
		{
		case vk::Format::eR8Unorm: return FramebufferTexFormat::R8;
		case vk::Format::eR8G8Unorm: return FramebufferTexFormat::RG8;
		case vk::Format::eR8G8B8Unorm: return FramebufferTexFormat::RGB8;
		case vk::Format::eR8G8B8A8Unorm: return FramebufferTexFormat::RGBA8;
		case vk::Format::eD32Sfloat: return FramebufferTexFormat::D32F;
		case vk::Format::eD32SfloatS8Uint: return FramebufferTexFormat::D32FS8U;
		case vk::Format::eD24UnormS8Uint: return FramebufferTexFormat::D24UNS8U;
		default:
			assert(false);
		}
	}
	RenderTexture::RenderTexture(uint32_t width, uint32_t height, FramebufferTexFormat colorFormat)
	{
		mFormat = colorFormat;
		auto api = (RendererApi*)(RendererApi::Get().get());

		vk::Format internalFormat;
		bool isLinear;
		bool isDepth;

		std::tie(internalFormat, isLinear, isDepth) = formatToVk(colorFormat, api->mPhysicalDevice);

		mFormat = colorFormat;
		mInternalFormat = vkToFormat(internalFormat);
		auto formatProps = api->mPhysicalDevice.getFormatProperties(internalFormat);

		vk::ImageCreateInfo info({},
			vk::ImageType::e2D, 
			internalFormat,
			vk::Extent3D(width, height, 1), 
			1, 1,
			vk::SampleCountFlagBits::e1, 
			isLinear ? vk::ImageTiling::eLinear : vk::ImageTiling::eOptimal,
			(isDepth ? vk::ImageUsageFlagBits::eDepthStencilAttachment : vk::ImageUsageFlagBits::eColorAttachment) | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst,
			vk::SharingMode::eExclusive);
		info.setInitialLayout(vk::ImageLayout::eUndefined);
		
		vma::AllocationCreateInfo allocInfo;
		
		std::tie(mImage, mAllocation) = api->mAllocator.createImage(info, allocInfo);
	}
	RenderTexture::~RenderTexture()
	{
		auto api = (RendererApi*)(RendererApi::Get().get());

		api->mDevice.destroy(mImage);
	}
}
