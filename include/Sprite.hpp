#pragma once

#include "Rendering/Object.hpp"
#include "Rendering/ShaderProgram.hpp"

#include <string>

class Sprite : public Object
{
	using PositionAttribute = GLMVertexAttribute<0, glm::vec2>;
	using ColorAttribute    = GLMVertexAttribute<1, glm::vec3>;
	using UVAttribute       = GLMVertexAttribute<2, glm::vec2>;

	using Texture0Uniform   = GLMShaderUniform<"Texture0", GLuint>;
	using TransformUniform  = GLMShaderUniform<"PassedTransform", glm::mat4>;
	using ProjectionUniform = GLMShaderUniform<"PassedProjection", glm::mat4>;


	public:
		struct SpriteVertexData
		{
			glm::vec2 Position;
			glm::vec3 Color;
			glm::vec2 UV;
		};

		using SpriteSPType = ShaderProgram<
			SpriteVertexData,
			VertexAttributesList<PositionAttribute, ColorAttribute, UVAttribute>,
			ShaderUniformsList<Texture0Uniform, TransformUniform, ProjectionUniform>
		>;

		// clang-format off
		inline static const std::array<SpriteVertexData, 4> SpriteVertices = {
			SpriteVertexData
			{
				.Position = { -.5f, .5f },
				.Color    = { 1.0f, 1.0f, 1.0f },
				.UV       = { 0.0f, 0.0f }
			},
			{
				.Position = { .5f, .5f },
				.Color    = { 1.0f, 1.0f, 1.0f },
				.UV       = { 1.0f, 0.0f }
			},
			{
				.Position = { .5f, -.5f },
				.Color    = { 1.0f, 1.0f, 1.0f },
				.UV       = { 1.0f, 1.0f }
			},
			{
				.Position = { -.5f, -.5f },
				.Color    = { 1.0f, 1.0f, 1.0f },
				.UV       = { 0.0f, 1.0f }
			}
		};
		// clang-format on

	public:
		explicit Sprite(const std::string_view& ImagePath);
		~Sprite() override;

		void Init() override;
		void LoadImage(const std::string_view& ImagePath);

		void Update(const float DeltaTime) override;
		void Render(const glm::mat4& Projection) override;


	public:
		std::string Name;
		glm::vec2   SpriteTexSize{ 1.f };

		glm::vec2 Location{ 0.f };
		float     Rotation = 0.f;
		glm::vec2 Scale{ 1.f };

		float ZDepth = 0.f;


	private:
		SpriteSPType* SpriteRenderer;
		uint32_t      TextureID = 0;
};
