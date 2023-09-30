#include "VMaterial.h"
#include "VUniformBuffer.h"

namespace RoxEngine::Vulkan
{
	Material::Material(std::shared_ptr<RoxEngine::Shader> shader)
	{
		mShader = std::dynamic_pointer_cast<Shader>(shader);

		for (auto& desc : mShader->mUbos)
		{
			mUbos.insert({ desc.name, std::make_shared<UniformBuffer>(desc) });
		}
	}

	std::shared_ptr<RoxEngine::Shader> Material::GetShader()
	{
		return mShader;
	}

	std::shared_ptr<RoxEngine::UniformBuffer> Material::GetUbo(const std::string& name)
	{
		return mUbos.at(name);
	}

}