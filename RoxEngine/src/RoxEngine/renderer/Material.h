#pragma once
#include <RoxEngine/renderer/Shader.h>
namespace RoxEngine {
	class Material
	{
	public:
		static std::shared_ptr<Material> Create(std::shared_ptr<Shader> shader);

		virtual std::shared_ptr<Shader> GetShader() = 0;
	};
}
