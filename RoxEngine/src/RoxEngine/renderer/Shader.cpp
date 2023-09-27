#include "Shader.h"
#include <regex>
#include <unordered_map>
#include <RoxEngine/core/Logger.h>
#include <fstream>
#include <RoxEngine/renderer/RendererApi.h>
#include <RoxEngine/core/Assert.h>

#include <Platform/Vulkan/VShader.h>

namespace RoxEngine {
	std::unordered_map<std::string, std::string> PreprocessShader(const std::string& input) {
		std::regex shader_decl("#shader ([a-zA-Z_]+)");

		std::sregex_iterator currentMatch(input.begin(), input.end(), shader_decl);
		std::sregex_iterator end;

		bool first_time = true;
		std::unordered_map<std::string, std::string> shaderStages;
		while (currentMatch != end)
		{
			auto match = *currentMatch;

			if (first_time && match.prefix().str().size() > 0)
			{
				shaderStages[""] = match.prefix().str();
				first_time = false;
			}
			static std::regex shadercode_pattern("((?!#shader)[\\s\\S])*");

			std::smatch code;
			uint32_t code_start = match.position() + match.length();

			auto begin = input.begin() + match.position() + match.length();
			auto end = input.end();

			std::string shader_code = std::string(std::string_view(&*begin, (uint32_t)std::distance(begin, end)));

			std::regex_search(shader_code, code, shadercode_pattern);
			
			shaderStages[match[1].str()] = code[0].str();
			currentMatch++;
		}

		return shaderStages;
	}
	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		std::string shaderCode;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				shaderCode.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&shaderCode[0], size);
			}
			else
			{
				RE_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			RE_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		auto stages = PreprocessShader(shaderCode);
		std::vector<Option> options;
		if (stages.find("") != stages.end()) {
			std::string& option_raw = stages.at("");
			
			std::regex option("#option\\s+([a-zA-Z_]+)\\s+([a-zA-Z0-9_]+)");
			std::sregex_iterator currentMatch(option_raw.begin(), option_raw.end(), option);
			std::sregex_iterator end;
			while (currentMatch != end)
			{
				std::smatch match = *currentMatch;
				options.push_back({ match[1].str(), match[2].str() });
				currentMatch++;
			}
			stages.erase("");
		}
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::Shader>(stages, options);
		default:
			RE_CORE_ASSERT(false, fmt::format("RendererApi::Init() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
		return nullptr;
	}
	std::shared_ptr<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc, const std::vector<Option>& options)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::Shader>(vertexSrc, fragmentSrc,options);
		default:
			RE_CORE_ASSERT(false, fmt::format("RendererApi::Init() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
		return nullptr;
	}
}
