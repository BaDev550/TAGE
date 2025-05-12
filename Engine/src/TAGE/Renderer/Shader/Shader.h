#pragma once
#include "TAGE/Core/AssetManager/Asset.h"
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace TAGE::RENDERER {
	class Shader : public ASSET::Asset
	{
	public:
		Shader(const std::string& vertex, const std::string& fragment);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, const glm::vec2& value);
		void SetUniform(const std::string& name, const glm::vec3& value);
		void SetUniform(const std::string& name, const glm::mat4& value);
		void SetUniformArray(const std::string& name, const glm::mat4* data, int count);

		inline uint32_t GetProgramID() const { return _ProgramID; }
		virtual ASSET::AssetType GetType() const override { return ASSET::AssetType::SHADER; }
	private:
		uint32_t _ProgramID = 0;

		std::string ParseShaderFile(const std::string& path);
		uint32_t CompileShader(uint32_t type, const std::string& source);
		uint32_t CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc);

		int GetUniformLocation(const std::string& name);
	};
}