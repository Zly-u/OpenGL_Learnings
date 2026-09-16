#pragma once

#include "Rendering/Object.hpp"
#include "Rendering/ShaderProgram.hpp"

class SlidingSprite : public Object
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
			PositionAttribute Position;
			ColorAttribute Color;
			UVAttribute UV;
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
		explicit SlidingSprite(const std::string_view& ImagePath);
		~SlidingSprite() override;

		void Init() override;

		void Update(const float DeltaTime) override;
		void Render(const glm::mat4& Projection) override;
};
