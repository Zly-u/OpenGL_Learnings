#pragma once

#include "Rendering/Object.hpp"
#include "Rendering/ShaderProgram.hpp"

class Solid : public Object
{
	using PositionAttribute = GLMVertexAttribute<0, glm::vec2>;
	using ColorAttribute    = GLMVertexAttribute<1, glm::vec3>;

	using TransformUniform  = GLMShaderUniform<"PassedTransform", glm::mat4>;
	using ProjectionUniform = GLMShaderUniform<"PassedProjection", glm::mat4>;


	public:
		struct SpriteVertexData
		{
			PositionAttribute Position;
			ColorAttribute Color;
		};

		using ShaderProgram_Solid = ShaderProgram<
			SpriteVertexData,
			VertexAttributesList<PositionAttribute, ColorAttribute>,
			ShaderUniformsList<TransformUniform, ProjectionUniform>
		>;

		// clang-format off
		inline static const std::array<SpriteVertexData, 4> SolidVertices = {
			SpriteVertexData
			{
				.Position = { -.5f, .5f },
				.Color    = { 1.0f, 0.0f, 0.0f },
			},
			{
				.Position = { .5f, .5f },
				.Color    = { 0.0f, 1.0f, 0.0f },
			},
			{
				.Position = { .5f, -.5f },
				.Color    = { 0.0f, 0.0f, 1.0f },
			},
			{
				.Position = { -.5f, -.5f },
				.Color    = { 1.0f, 1.0f, 1.0f },
			}
		};
		// clang-format on

	public:
		explicit Solid();
		~Solid() override;

		void Init() override;

		void Update(const float DeltaTime) override;
		void Render(const glm::mat4& Projection) override;
};
