#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace RoxEngine
{
	class UniformBuffer {
	public:
		bool Set(std::string name, bool value);
		bool Set(std::string name, int8_t value);
		bool Set(std::string name, uint8_t value);
		bool Set(std::string name, int16_t value);
		bool Set(std::string name, uint16_t value);
		bool Set(std::string name, int32_t value);
		bool Set(std::string name, uint32_t value);
		bool Set(std::string name, int64_t value);
		bool Set(std::string name, uint64_t value);
		bool Set(std::string name, float value);
		bool Set(std::string name, double value);


		bool Set(std::string name, glm::mat2 value);
		bool Set(std::string name, glm::mat3 value);
		bool Set(std::string name, glm::mat4 value);


		bool Set(std::string name, glm::vec2 value);
		bool Set(std::string name, glm::vec3 value);
		bool Set(std::string name, glm::vec4 value);
		bool Set(std::string name, glm::ivec2 value);
		bool Set(std::string name, glm::ivec3 value);
		bool Set(std::string name, glm::ivec4 value);
		bool Set(std::string name, glm::uvec2 value);
		bool Set(std::string name, glm::uvec3 value);
		bool Set(std::string name, glm::uvec4 value);
		bool Set(std::string name, glm::bvec2 value);
		bool Set(std::string name, glm::bvec3 value);
		bool Set(std::string name, glm::bvec4 value);

		struct UboDesc {
			enum class Type {
				Bool,
				SByte,
				UByte,
				Short,
				UShort,
				Int,
				UInt,
				Int64,
				UInt64,
				Float,
				Double,

				Mat2,
				Mat3,
				Mat4,

				Vec2,
				Vec3,
				Vec4,
				iVec2,
				iVec3,
				iVec4,
				uVec2,
				uVec3,
				uVec4,
				bVec2,
				bVec3,
				bVec4,

				Struct,
			};
			struct MemberDesc {
				size_t offset;
				Type type;
			};
			size_t mSize;
			uint32_t mBinding;
			std::string name;
			std::unordered_map<std::string, MemberDesc> mFields;
		};


		UniformBuffer(UboDesc desc);
		virtual ~UniformBuffer();

		bool mChanged;
		UboDesc mDesc;
		uint8_t* mData;
	};
}