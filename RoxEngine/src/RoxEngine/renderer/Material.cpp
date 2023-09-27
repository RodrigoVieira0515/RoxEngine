#include "Material.h"
#include <RoxEngine/renderer/RendererApi.h>
#include <RoxEngine/core/Assert.h>
#include <Platform/Vulkan/VMaterial.h>

namespace RoxEngine
{
    std::shared_ptr<Material> Material::Create(std::shared_ptr<Shader> shader)
    {
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::Material>(shader);
		default:
			RE_CORE_ASSERT(false, fmt::format("Material::Create() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
		return nullptr;
    }
}
