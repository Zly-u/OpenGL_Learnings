#pragma once

#include "Rendering/ShaderProgram.hpp"

#include <string>


class Sprite
{
	using PositionAttribute = GLMVertexAttribute<0, glm::vec2>;
	using ColorAttribute    = GLMVertexAttribute<1, glm::vec3>;
	using UVAttribute       = GLMVertexAttribute<2, glm::vec2>;

	using Texture0Uniform = GLMShaderUniform<"Texture0", GLuint>;
	using TransformUniform = GLMShaderUniform<"PassedTransform", glm::mat4>;
	using ProjectionUniform = GLMShaderUniform<"PassedProjection", glm::mat4>;

	public:
		struct SpriteVertexData
		{
			glm::vec2 Position;
			glm::vec3 Color;
			glm::vec2 UV;
		};

		using SpriteSP = ShaderProgram<
			SpriteVertexData,
			VertexAttributesList<PositionAttribute, ColorAttribute, UVAttribute>,
			ShaderUniformsList<Texture0Uniform, TransformUniform, ProjectionUniform>
		>;

	public:
		explicit Sprite(const std::string_view& ImagePath);

		void Init();
		void LoadImage(const std::string_view& ImagePath);

		void Update(const float DeltaTime);
		void Render(const glm::mat4& Projection);


	public:
		std::string Name;
		glm::vec2 SpriteTexSize{1.f};

		glm::vec2 Location{0.f};
		float Rotation = 0.f;
		glm::vec2 Scale{1.f};

		float ZDepth = 0.f;


	private:
		SpriteSP* SpriteRenderer;
};
