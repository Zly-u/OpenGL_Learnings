#pragma once

#include "Sprite.hpp"

class SpritePixelization : public Object
{
	using PositionAttribute = GLMVertexAttribute<0, glm::vec2>;
	using ColorAttribute    = GLMVertexAttribute<1, glm::vec3>;
	using UVAttribute       = GLMVertexAttribute<2, glm::vec2>;

	using TransformUniform  = GLMShaderUniform<"PassedTransform", glm::mat4>;
	using ProjectionUniform = GLMShaderUniform<"PassedProjection", glm::mat4>;
	using Texture0Uniform   = GLMShaderUniform<"Texture0", GLuint>;
	using PixelationUniform = GLMShaderUniform<"PixelationLevel", float>;


	public:
		struct SpriteVertexData
		{
			glm::vec2 Position;
			glm::vec3 Color;
			glm::vec2 UV;
		};

		using SpritePxSPType = ShaderProgram<
			SpriteVertexData,
			VertexAttributesList<PositionAttribute, ColorAttribute, UVAttribute>,
			ShaderUniformsList<Texture0Uniform, TransformUniform, ProjectionUniform, PixelationUniform>
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
		explicit SpritePixelization(const std::string_view& ImagePath);
		~SpritePixelization() override;

		void Init() override;

		void Update(const float DeltaTime) override;
		void Render(const glm::mat4& Projection) override;


	private:
		float PixelationLevel = 1.f;
};