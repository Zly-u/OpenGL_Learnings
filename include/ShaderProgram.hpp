#pragma once
#include <string_view>
#include <glad/glad.h>


class ShaderProgram
{
	public:
		ShaderProgram() = default;
		ShaderProgram(const std::string_view& VertexShader, const std::string_view& FragmentShader);
		ShaderProgram(const ShaderProgram& OtherShaderProgram);
		int64_t LoadShaderFromFile(const std::string_view& ShaderFile, const GLenum ShaderType);
		void Render(uint32_t VAO, uint32_t EBO);


	private:
		uint32_t ShaderProgramID;
};
