#pragma once

#include "Logging.h"
#include "Random.h"

#include "TVertexArrayObjectHandler.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "ShaderUniformsDescriptor.hpp"

#include <glm/gtx/string_cast.hpp>

#include <fstream>

#include <array>
#include <glad/glad.h>
#include <string_view>

// ---------------------------------------------------------------------------------------

class ShaderProgramBase
{
	public:
		ShaderProgramBase(
			const std::string_view& VertexShader,
			const std::string_view& FragmentShader
		)
		{
			Log::println("Created ShaderProgram");
			ShaderProgramBase::PrepareShaders(VertexShader, FragmentShader);
			ShaderProgramBase::PreparePolygon();
		}

		virtual ~ShaderProgramBase(){}

	public:
		virtual void Use()
		{
			glUseProgram(ShaderProgramID);
		}

		using GraphicsUpdatingFuncSign = std::function<void(ShaderProgramBase*)>;
		virtual void Render(
			const GraphicsUpdatingFuncSign& GraphicsUpdateFunc,
			const GraphicsUpdatingFuncSign& DeinitializeGraphicsFunc
		) = 0;

	public:
		__forceinline void SetTransform(const glm::mat4& NewTransform)
		{
			Transform = NewTransform;
		}

		__forceinline void SetProjection(const glm::mat4& NewProjection)
		{
			Projection = NewProjection;
		}


	protected:
		[[nodiscard]] int64_t
		LoadShaderFromFile(const std::string_view& ShaderFile, const GLenum ShaderType) const;

		virtual void PrepareShaders(const std::string_view& VertexShader, const std::string_view& FragmentShader) {}
		virtual void PreparePolygon() {}

	public:
		uint32_t ShaderProgramID = 0;


	public:
		glm::mat4 Transform{ 1.f };
		glm::mat4 Projection{ 1.f };

};

// ---------------------------------------------------------------------------------------

template<typename VertexDataType, typename TAttributeListType, typename TUniformsListType>
class ShaderProgram final : public ShaderProgramBase
{
	public:
		ShaderProgram(
			const std::string_view& VertexShader,
			const std::string_view& FragmentShader,
			const std::array<VertexDataType, 4>& NewVertices
		) : ShaderProgramBase(VertexShader, FragmentShader), Vertices(NewVertices)
		{
			ShaderProgram::PrepareShaders(VertexShader, FragmentShader);
			ShaderProgram::PreparePolygon();
		}

		~ShaderProgram() override
		{
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
		}

		// ---------------------------------------------------------------------------------------

		void Render(
			const GraphicsUpdatingFuncSign& GraphicsUpdateFunc,
			const GraphicsUpdatingFuncSign& DeinitializeGraphicsFunc
		) override
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			Use();

			VAO_Handler.Bind();

			GraphicsUpdateFunc(this);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// Unbind stuff
			glBindVertexArray(0);

			DeinitializeGraphicsFunc(this);

			glUseProgram(0);
		}


	protected:
		void PrepareShaders(const std::string_view& VertexShader, const std::string_view& FragmentShader) override
		{
			const int64_t VertexShaderID = LoadShaderFromFile(VertexShader, GL_VERTEX_SHADER);
			if (VertexShaderID == -1)
			{
				return;
			}

			const int64_t FragmentShaderID = LoadShaderFromFile(FragmentShader, GL_FRAGMENT_SHADER);
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

			UniformsDescriptor.SetShaderProgramID(ShaderProgramID);
		}

		void PreparePolygon() override
		{
			static const uint32_t Indices[] = {
				0, 1, 2,    // first triangle
				0, 2, 3     // second triangle
			};

			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			VAO_Handler.Bind();

			// Vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices.data(), GL_STATIC_DRAW);

			// Element buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

			VAO_Handler.SetupVertexAttributes();

			VAO_Handler.Unbind();
		}

	public:
		VertexArrayObjectHandler<VertexDataType, TAttributeListType> VAO_Handler;
		ShaderUniformsDescriptor<TUniformsListType> UniformsDescriptor;

		uint32_t VBO = 0;
		uint32_t EBO = 0;

	public:
		const std::array<VertexDataType, 4>& Vertices;
};
