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


	public:
		uint32_t VBO = 0;
		uint32_t VAO = 0;
		uint32_t EBO = 0;

		uint32_t TextureID = 0;

		uint32_t ShaderProgramID = 0;

		int32_t Uniform_VertexColorLocation;
};
