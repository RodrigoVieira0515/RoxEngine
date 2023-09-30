#include <RoxEngine/renderer/UniformBuffer.h>

namespace RoxEngine
{
	UniformBuffer::UniformBuffer(UboDesc desc)
	{
		mDesc = desc;
		mData = (uint8_t*)malloc(desc.mSize);
	}

	UniformBuffer::~UniformBuffer()
	{
		if(mData)
			free(mData);
	}
	#define UPLOAD_PRIMITIVE(out_type) out_type va = value; std::memcpy((uint8_t*)mData + field.offset, &va, sizeof(out_type)); mChanged = true; 

	bool UniformBuffer::Set(std::string name, bool v)
	{
		uint32_t value = 0xFFFFFFFF;
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
			case UboDesc::Type::Bool:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
			case UboDesc::Type::SByte:  { UPLOAD_PRIMITIVE(int8_t)   return true; }
			case UboDesc::Type::UByte:  { UPLOAD_PRIMITIVE(uint8_t)  return true; }
			case UboDesc::Type::Short:  { UPLOAD_PRIMITIVE(int16_t)  return true; }
			case UboDesc::Type::UShort: { UPLOAD_PRIMITIVE(uint16_t) return true; }
			case UboDesc::Type::Int:    { UPLOAD_PRIMITIVE(int32_t)  return true; }
			case UboDesc::Type::UInt:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
			case UboDesc::Type::Int64:  { UPLOAD_PRIMITIVE(int64_t)  return true; }
			case UboDesc::Type::UInt64: { UPLOAD_PRIMITIVE(uint64_t) return true; }
			case UboDesc::Type::Float:  { UPLOAD_PRIMITIVE(float)    return true; }
			case UboDesc::Type::Double: { UPLOAD_PRIMITIVE(double)   return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, int8_t value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Bool:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::SByte:  { UPLOAD_PRIMITIVE(int8_t)   return true; }
		case UboDesc::Type::UByte:  { UPLOAD_PRIMITIVE(uint8_t)  return true; }
		case UboDesc::Type::Short:  { UPLOAD_PRIMITIVE(int16_t)  return true; }
		case UboDesc::Type::UShort: { UPLOAD_PRIMITIVE(uint16_t) return true; }
		case UboDesc::Type::Int:    { UPLOAD_PRIMITIVE(int32_t)  return true; }
		case UboDesc::Type::UInt:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::Int64:  { UPLOAD_PRIMITIVE(int64_t)  return true; }
		case UboDesc::Type::UInt64: { UPLOAD_PRIMITIVE(uint64_t) return true; }
		case UboDesc::Type::Float:  { UPLOAD_PRIMITIVE(float)    return true; }
		case UboDesc::Type::Double: { UPLOAD_PRIMITIVE(double)   return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, uint8_t value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Bool:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::SByte:  { UPLOAD_PRIMITIVE(int8_t)   return true; }
		case UboDesc::Type::UByte:  { UPLOAD_PRIMITIVE(uint8_t)  return true; }
		case UboDesc::Type::Short:  { UPLOAD_PRIMITIVE(int16_t)  return true; }
		case UboDesc::Type::UShort: { UPLOAD_PRIMITIVE(uint16_t) return true; }
		case UboDesc::Type::Int:    { UPLOAD_PRIMITIVE(int32_t)  return true; }
		case UboDesc::Type::UInt:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::Int64:  { UPLOAD_PRIMITIVE(int64_t)  return true; }
		case UboDesc::Type::UInt64: { UPLOAD_PRIMITIVE(uint64_t) return true; }
		case UboDesc::Type::Float:  { UPLOAD_PRIMITIVE(float)    return true; }
		case UboDesc::Type::Double: { UPLOAD_PRIMITIVE(double)   return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, int16_t value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Bool:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::SByte:  { UPLOAD_PRIMITIVE(int8_t)   return true; }
		case UboDesc::Type::UByte:  { UPLOAD_PRIMITIVE(uint8_t)  return true; }
		case UboDesc::Type::Short:  { UPLOAD_PRIMITIVE(int16_t)  return true; }
		case UboDesc::Type::UShort: { UPLOAD_PRIMITIVE(uint16_t) return true; }
		case UboDesc::Type::Int:    { UPLOAD_PRIMITIVE(int32_t)  return true; }
		case UboDesc::Type::UInt:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::Int64:  { UPLOAD_PRIMITIVE(int64_t)  return true; }
		case UboDesc::Type::UInt64: { UPLOAD_PRIMITIVE(uint64_t) return true; }
		case UboDesc::Type::Float:  { UPLOAD_PRIMITIVE(float)    return true; }
		case UboDesc::Type::Double: { UPLOAD_PRIMITIVE(double)   return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, uint16_t value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Bool:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::SByte:  { UPLOAD_PRIMITIVE(int8_t)   return true; }
		case UboDesc::Type::UByte:  { UPLOAD_PRIMITIVE(uint8_t)  return true; }
		case UboDesc::Type::Short:  { UPLOAD_PRIMITIVE(int16_t)  return true; }
		case UboDesc::Type::UShort: { UPLOAD_PRIMITIVE(uint16_t) return true; }
		case UboDesc::Type::Int:    { UPLOAD_PRIMITIVE(int32_t)  return true; }
		case UboDesc::Type::UInt:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::Int64:  { UPLOAD_PRIMITIVE(int64_t)  return true; }
		case UboDesc::Type::UInt64: { UPLOAD_PRIMITIVE(uint64_t) return true; }
		case UboDesc::Type::Float:  { UPLOAD_PRIMITIVE(float)    return true; }
		case UboDesc::Type::Double: { UPLOAD_PRIMITIVE(double)   return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, int32_t value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Bool:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::SByte:  { UPLOAD_PRIMITIVE(int8_t)   return true; }
		case UboDesc::Type::UByte:  { UPLOAD_PRIMITIVE(uint8_t)  return true; }
		case UboDesc::Type::Short:  { UPLOAD_PRIMITIVE(int16_t)  return true; }
		case UboDesc::Type::UShort: { UPLOAD_PRIMITIVE(uint16_t) return true; }
		case UboDesc::Type::Int:    { UPLOAD_PRIMITIVE(uint16_t) return true; }
		case UboDesc::Type::UInt:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::Int64:  { UPLOAD_PRIMITIVE(int64_t)  return true; }
		case UboDesc::Type::UInt64: { UPLOAD_PRIMITIVE(uint64_t) return true; }
		case UboDesc::Type::Float:  { UPLOAD_PRIMITIVE(float)    return true; }
		case UboDesc::Type::Double: { UPLOAD_PRIMITIVE(double)   return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, uint32_t value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Bool:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::SByte:  { UPLOAD_PRIMITIVE(int8_t)   return true; }
		case UboDesc::Type::UByte:  { UPLOAD_PRIMITIVE(uint8_t)  return true; }
		case UboDesc::Type::Short:  { UPLOAD_PRIMITIVE(int16_t)  return true; }
		case UboDesc::Type::UShort: { UPLOAD_PRIMITIVE(uint16_t) return true; }
		case UboDesc::Type::Int:    { UPLOAD_PRIMITIVE(int32_t)  return true; }
		case UboDesc::Type::UInt:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::Int64:  { UPLOAD_PRIMITIVE(int64_t)  return true; }
		case UboDesc::Type::UInt64: { UPLOAD_PRIMITIVE(uint64_t) return true; }
		case UboDesc::Type::Float:  { UPLOAD_PRIMITIVE(float)    return true; }
		case UboDesc::Type::Double: { UPLOAD_PRIMITIVE(double)   return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, int64_t value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Bool:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::SByte:  { UPLOAD_PRIMITIVE(int8_t)   return true; }
		case UboDesc::Type::UByte:  { UPLOAD_PRIMITIVE(uint8_t)  return true; }
		case UboDesc::Type::Short:  { UPLOAD_PRIMITIVE(int16_t)  return true; }
		case UboDesc::Type::UShort: { UPLOAD_PRIMITIVE(uint16_t) return true; }
		case UboDesc::Type::Int:    { UPLOAD_PRIMITIVE(int32_t)  return true; }
		case UboDesc::Type::UInt:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::Int64:  { UPLOAD_PRIMITIVE(int64_t)  return true; }
		case UboDesc::Type::UInt64: { UPLOAD_PRIMITIVE(uint64_t) return true; }
		case UboDesc::Type::Float:  { UPLOAD_PRIMITIVE(float)    return true; }
		case UboDesc::Type::Double: { UPLOAD_PRIMITIVE(double)   return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, uint64_t value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Bool:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::SByte:  { UPLOAD_PRIMITIVE(int8_t)   return true; }
		case UboDesc::Type::UByte:  { UPLOAD_PRIMITIVE(uint8_t)  return true; }
		case UboDesc::Type::Short:  { UPLOAD_PRIMITIVE(int16_t)  return true; }
		case UboDesc::Type::UShort: { UPLOAD_PRIMITIVE(uint16_t) return true; }
		case UboDesc::Type::Int:    { UPLOAD_PRIMITIVE(int32_t)  return true; }
		case UboDesc::Type::UInt:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::Int64:  { UPLOAD_PRIMITIVE(int64_t)  return true; }
		case UboDesc::Type::UInt64: { UPLOAD_PRIMITIVE(uint64_t) return true; }
		case UboDesc::Type::Float:  { UPLOAD_PRIMITIVE(float)    return true; }
		case UboDesc::Type::Double: { UPLOAD_PRIMITIVE(double)   return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, float value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Bool:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::SByte:  { UPLOAD_PRIMITIVE(int8_t)   return true; }
		case UboDesc::Type::UByte:  { UPLOAD_PRIMITIVE(uint8_t)  return true; }
		case UboDesc::Type::Short:  { UPLOAD_PRIMITIVE(int16_t)  return true; }
		case UboDesc::Type::UShort: { UPLOAD_PRIMITIVE(uint16_t) return true; }
		case UboDesc::Type::Int:    { UPLOAD_PRIMITIVE(int32_t)  return true; }
		case UboDesc::Type::UInt:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::Int64:  { UPLOAD_PRIMITIVE(int64_t)  return true; }
		case UboDesc::Type::UInt64: { UPLOAD_PRIMITIVE(uint64_t) return true; }
		case UboDesc::Type::Float:  { UPLOAD_PRIMITIVE(float)    return true; }
		case UboDesc::Type::Double: { UPLOAD_PRIMITIVE(double)   return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, double value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Bool:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::SByte:  { UPLOAD_PRIMITIVE(int8_t)   return true; }
		case UboDesc::Type::UByte:  { UPLOAD_PRIMITIVE(uint8_t)  return true; }
		case UboDesc::Type::Short:  { UPLOAD_PRIMITIVE(int16_t)  return true; }
		case UboDesc::Type::UShort: { UPLOAD_PRIMITIVE(uint16_t) return true; }
		case UboDesc::Type::Int:    { UPLOAD_PRIMITIVE(int32_t)  return true; }
		case UboDesc::Type::UInt:   { UPLOAD_PRIMITIVE(uint32_t) return true; }
		case UboDesc::Type::Int64:  { UPLOAD_PRIMITIVE(int64_t)  return true; }
		case UboDesc::Type::UInt64: { UPLOAD_PRIMITIVE(uint64_t) return true; }
		case UboDesc::Type::Float:  { UPLOAD_PRIMITIVE(float)    return true; }
		case UboDesc::Type::Double: { UPLOAD_PRIMITIVE(double)   return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::mat2 value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;

		switch (field.type)
		{
		case UboDesc::Type::Mat2: { UPLOAD_PRIMITIVE(glm::mat2) return true; }
		case UboDesc::Type::Mat3: { UPLOAD_PRIMITIVE(glm::mat3) return true; }
		case UboDesc::Type::Mat4: { UPLOAD_PRIMITIVE(glm::mat4) return true; }
		default:
			break;
		}

		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::mat3 value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Mat2: { UPLOAD_PRIMITIVE(glm::mat2) return true; }
		case UboDesc::Type::Mat3: { UPLOAD_PRIMITIVE(glm::mat3) return true; }
		case UboDesc::Type::Mat4: { UPLOAD_PRIMITIVE(glm::mat4) return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::mat4 value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Mat2: { UPLOAD_PRIMITIVE(glm::mat2) return true; }
		case UboDesc::Type::Mat3: { UPLOAD_PRIMITIVE(glm::mat3) return true; }
		case UboDesc::Type::Mat4: { UPLOAD_PRIMITIVE(glm::mat4) return true; }
		default:
			break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::vec2 v)
	{
		glm::vec4 value = glm::vec4(v, 0,0);
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::vec3 v)
	{
		glm::vec4 value = glm::vec4(v, 0);
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::vec4 value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::ivec2 v)
	{
		glm::ivec4 value = glm::ivec4(v, 0, 0);
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::ivec3 v)
	{
		glm::ivec4 value = glm::ivec4(v, 0);
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::ivec4 value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::uvec2 v)
	{
		glm::uvec4 value = glm::uvec4(v, 0, 0);
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::uvec3 v)
	{
		glm::uvec4 value = glm::uvec4(v, 0);
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::uvec4 value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::bvec2 v)
	{
		glm::bvec4 value = glm::bvec4(v, false,false);
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::bvec3 v)
	{
		glm::bvec4 value = glm::bvec4(v, false);
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

	bool UniformBuffer::Set(std::string name, glm::bvec4 value)
	{
		auto& it = mDesc.mFields.find(name);
		if (it == mDesc.mFields.end())
			return false;
		auto& field = it->second;
		switch (field.type)
		{
		case UboDesc::Type::Vec2:  { UPLOAD_PRIMITIVE(glm::vec2) return true; }
		case UboDesc::Type::Vec3:  { UPLOAD_PRIMITIVE(glm::vec3) return true; }
		case UboDesc::Type::Vec4:  { UPLOAD_PRIMITIVE(glm::vec4) return true; }
		case UboDesc::Type::iVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::iVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::iVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		case UboDesc::Type::uVec2: { UPLOAD_PRIMITIVE(glm::uvec2) return true; }
		case UboDesc::Type::uVec3: { UPLOAD_PRIMITIVE(glm::uvec3) return true; }
		case UboDesc::Type::uVec4: { UPLOAD_PRIMITIVE(glm::uvec4) return true; }
		case UboDesc::Type::bVec2: { UPLOAD_PRIMITIVE(glm::ivec2) return true; }
		case UboDesc::Type::bVec3: { UPLOAD_PRIMITIVE(glm::ivec3) return true; }
		case UboDesc::Type::bVec4: { UPLOAD_PRIMITIVE(glm::ivec4) return true; }
		default: break;
		}
		return false;
	}

}