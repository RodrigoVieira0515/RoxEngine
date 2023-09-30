#pragma once
#include <string>
#include <memory>
#include <vector>
namespace RoxEngine {
	
	class Shader
	{
	public:
		enum Type {
			FRAGMENT,
			VERTEX,
		};
		struct Option {
			std::string name;
			std::string value;
		};

		virtual uint32_t GetNumOfUbos() = 0;

		static std::shared_ptr<Shader> Create(const std::string& filepath);
		static std::shared_ptr<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc, const std::vector<Option>& options);
		virtual ~Shader() = default;
	};
}

