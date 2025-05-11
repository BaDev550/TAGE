#include "tagepch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include "ShaderPreprocessor.h"

namespace TAGE::RENDERER {
	Shader::Shader(const std::string& vertex, const std::string& fragment)
	{
		std::string vertexCode = ParseShaderFile(vertex);
		std::string fragmentCode = ParseShaderFile(fragment);

		_ProgramID = CreateProgram(vertexCode, fragmentCode);
	}
	Shader::~Shader()
	{
		glDeleteProgram(_ProgramID);
	}
	void Shader::Bind() const
	{
		glUseProgram(_ProgramID);
	}
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
	std::string Shader::ParseShaderFile(const std::string& path)
	{
		return ShaderPreprocessor::Preprocess(path);
	}
	uint32_t Shader::CompileShader(uint32_t type, const std::string& source)
	{
		uint32_t id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			std::string message(length, '\0');
			glGetShaderInfoLog(id, length, &length, &message[0]);
			CORE_LOG_ERROR("Shader compilation failed: {0}", message);
			glDeleteShader(id);
			return 0;
		}
		return id;
	}
	uint32_t Shader::CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		uint32_t program = glCreateProgram();
		uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
		uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			int length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			std::string error(length, '\0');
			glGetProgramInfoLog(program, length, &length, &error[0]);
			CORE_LOG_ERROR("Shader linking failed: {}", error);
			glDeleteShader(vs);
			glDeleteShader(fs);
			glDeleteProgram(program);
			return 0;
		}

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}
	int Shader::GetUniformLocation(const std::string& name)
	{
		int location = glGetUniformLocation(_ProgramID, name.c_str());
		return location;
	}

	void Shader::SetUniform(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}
	void Shader::SetUniform(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}
	void Shader::SetUniform(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}
	void Shader::SetUniform(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}
	void Shader::SetUniform(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
	}
}