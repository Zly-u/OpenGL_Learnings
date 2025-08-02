#pragma once
#include <array>
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <string_view>

struct VertexData
{
	glm::vec2 Position;
	glm::vec3 Color;
	glm::vec2 UV;
};

// TODO: Templated way of making Inputs and Outputs?
class ShaderProgram
{
	public:
		ShaderProgram(
			const std::string_view& VertexShader,
			const std::string_view& FragmentShader
		);
		ShaderProgram(
			const std::string_view&          VertexShader,
			const std::string_view&          FragmentShader,
			const std::array<VertexData, 4>& NewVertices
		);
		~ShaderProgram();

		void Use();
		void Render(const glm::mat4& Projection);


	public:
		void SetTransform(const glm::mat4& NewTransform);

	private:
		int64_t
		LoadShaderFromFile(const std::string_view& ShaderFile, const GLenum ShaderType);
		void FindUniforms();
		void PrepareShaders(
			const std::string_view& VertexShader,
			const std::string_view& FragmentShader
		);
		void PreparePolygon();


	public:
		uint32_t VBO = 0;
		uint32_t VAO = 0;
		uint32_t EBO = 0;

		uint32_t TextureID = 0;

		uint32_t ShaderProgramID = 0;

		int32_t Uniform_Texture_0;
		int32_t Uniform_Transform;
		int32_t Uniform_Projection;


	public:
		glm::mat4 Transform{ 1.f };


	private:
		std::array<VertexData, 4> Vertices = {
			VertexData{ .Position = { -.5f, .5f },
                       .Color    = { 1.0f, 1.0f, 1.0f },
                       .UV       = { 0.0f, 0.0f } },
			VertexData{ .Position = { .5f, .5f },
                       .Color    = { 1.0f, 1.0f, 1.0f },
                       .UV       = { 1.0f, 0.0f } },
			VertexData{ .Position = { .5f, -.5f },
                       .Color    = { 1.0f, 1.0f, 1.0f },
                       .UV       = { 1.0f, 1.0f } },
			VertexData{ .Position = { -.5f, -.5f },
                       .Color    = { 1.0f, 1.0f, 1.0f },
                       .UV       = { 0.0f, 1.0f } }
		};
};
