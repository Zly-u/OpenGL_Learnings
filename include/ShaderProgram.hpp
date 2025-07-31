#pragma once
#include <string_view>
#include <glad/glad.h>

// TODO: Templated way of making Inputs and Outputs?
class ShaderProgram
{
	public:
		ShaderProgram(const std::string_view& VertexShader, const std::string_view& FragmentShader);
		~ShaderProgram();

		void FindUniforms();

		int64_t LoadShaderFromFile(const std::string_view& ShaderFile, const GLenum ShaderType);
		void PreparePolygon();
		void Render();


	private:
		uint32_t VBO;
		uint32_t VAO;
		uint32_t EBO;

		uint32_t ShaderProgramID;

		int32_t Uniform_VertexColorLocation;
};
