#pragma once
#include <RoxEngine/renderer/Material.h>

namespace RoxEngine::Vulkan {
	class Material : public RoxEngine::Material
	{
	public:
		Material(std::shared_ptr<Shader> shader);
		std::shared_ptr<Shader> GetShader();
		
		std::shared_ptr<Shader> mShader;
	};
}
