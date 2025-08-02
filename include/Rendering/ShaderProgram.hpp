#pragma once

#include "Logging.h"
#include "Random.h"

#include "TVertexArrayObject.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <sstream>

#include <array>
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <string_view>


// template<typename UniformType, const char* UniformName>
// struct Uniform
// {
// 	using Type = UniformType;
//
// 	static constexpr const std::string_view Name = UniformName;
// 	GLint Location = -1;
// };


// ---------------------------------------------------------------------------------------

// TODO: Templated way of making Inputs and Outputs?
template<typename VertexDataType, typename... TGlAttributes>
class ShaderProgram
{
	public:
		ShaderProgram(
			const std::string_view&          VertexShader,
			const std::string_view&          FragmentShader,
			const std::array<VertexDataType, 4>& NewVertices
		) : Vertices(NewVertices)
		{
			PrepareShaders(VertexShader, FragmentShader);
			FindUniforms();
			PreparePolygon();
		}

		~ShaderProgram(){
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
		}

		// ---------------------------------------------------------------------------------------

		void Use(){
			glUseProgram(ShaderProgramID);
		}


		void Render(const glm::mat4& Projection){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			Use();

			VAO.Bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureID);

			glUniform1i(Uniform_Texture_0, 0); // 0 corresponds to GL_TEXTURE0

			glUniformMatrix4fv(Uniform_Transform, 1, GL_FALSE, glm::value_ptr(Transform));
			glUniformMatrix4fv(Uniform_Projection, 1, GL_FALSE, glm::value_ptr(Projection));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// Unbind stuff
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}


	public:
		void SetTransform(const glm::mat4& NewTransform){
			Transform = NewTransform;
		}

	private:
		int64_t
		LoadShaderFromFile(const std::string_view& ShaderFile, const GLenum ShaderType){
			std::ifstream infile(ShaderFile.data());

			if (infile.fail())
			{
				Log::println("Error opening shader file: {}", ShaderFile);
				return -1;
			}

			std::stringstream Buffer_ShaderShader;
			Buffer_ShaderShader << infile.rdbuf();
			std::string ShaderContent = Buffer_ShaderShader.str();

			const char* VertexShaderData      = ShaderContent.c_str();
			const GLint VertexShaderData_Size = static_cast<GLint>(ShaderContent.size());

			const GLuint NewShaderID = glCreateShader(ShaderType);
			glShaderSource(NewShaderID, 1, &VertexShaderData, &VertexShaderData_Size);
			glCompileShader(NewShaderID);

			int Success;
			glGetShaderiv(NewShaderID, GL_COMPILE_STATUS, &Success);
			if (!Success)
			{
				char infoLog[512];
				glGetShaderInfoLog(NewShaderID, 512, nullptr, infoLog);

				Log::println(
					"ERROR::SHADER::{}::COMPILATION_FAILED: {}",
					ShaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT",
					infoLog
				);

				return -1;
			}

			return NewShaderID;
		}
		void PrepareShaders(
			const std::string_view& VertexShader,
			const std::string_view& FragmentShader
			){
			const int64_t VertexShaderID = LoadShaderFromFile(VertexShader, GL_VERTEX_SHADER);
			if (VertexShaderID == -1)
			{
				return;
			}

			const int64_t FragmentShaderID =
				LoadShaderFromFile(FragmentShader, GL_FRAGMENT_SHADER);
			if (FragmentShaderID == -1)
			{
				return;
			}

			ShaderProgramID = glCreateProgram();
			glAttachShader(ShaderProgramID, VertexShaderID);
			glAttachShader(ShaderProgramID, FragmentShaderID);
			glLinkProgram(ShaderProgramID);

			int Success;
			glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &Success);
			if (!Success)
			{
				char infoLog[512];
				glGetProgramInfoLog(ShaderProgramID, 512, nullptr, infoLog);

				Log::println("ERROR::SHADERPROGRAM::LINKING_FAILED: {}", infoLog);

				return;
			}

			glDeleteShader(VertexShaderID);
			glDeleteShader(FragmentShaderID);
		}

		void FindUniforms()
		{
			Uniform_Texture_0 = glGetUniformLocation(ShaderProgramID, "Texture0");
			Uniform_Transform = glGetUniformLocation(ShaderProgramID, "PassedTransform");
			Uniform_Projection = glGetUniformLocation(ShaderProgramID, "PassedProjection");
		}

		void PreparePolygon(){
			static const uint32_t Indices[] = {
				0, 1, 2,   // first triangle
				0, 2, 3    // second triangle
			};

			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			VAO.Bind();

			// Vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices.data(), GL_STATIC_DRAW);

			// Element buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

			VAO.SetupVertexAttributes();

			VAO.Unbind();
		}

	public:
		VertexArrayObject<VertexDataType, TGlAttributes...> VAO;

		uint32_t VBO = 0;
		uint32_t EBO = 0;

		uint32_t TextureID = 0;

		uint32_t ShaderProgramID = 0;

		int32_t Uniform_Texture_0;
		int32_t Uniform_Transform;
		int32_t Uniform_Projection;


	public:
		glm::mat4 Transform{ 1.f };


	private:
		std::array<VertexDataType, 4> Vertices;
};
