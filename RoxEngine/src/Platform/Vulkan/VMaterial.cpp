#include "VMaterial.h"

namespace RoxEngine::Vulkan
{
	Material::Material(std::shared_ptr<Shader> shader)
	{
		mShader = shader;
	}

	std::shared_ptr<Shader> Material::GetShader()
	{
		return mShader;
	}

}