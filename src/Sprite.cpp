
#include "Sprite.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>


Sprite::Sprite(const std::string_view& ImagePath)
{
	static SpriteSPType StaticSpriteRenderer{
		"shaders/vertex.glsl",
		"shaders/fragment.glsl",
		SpriteVertices
	};
	SpriteRenderer = &StaticSpriteRenderer;

	LoadImage(ImagePath);
}

Sprite::~Sprite()
{

}


// -----------------------------------------------------------------------------------------------------------


void Sprite::Init()
{

}


void Sprite::Update(const float DeltaTime)
{

}


void Sprite::Render(const glm::mat4& Projection) {
	glm::mat4 Transform{1.0f};

	Transform = glm::translate(Transform, glm::vec3(Location.x, Location.y, ZDepth));
	Transform = glm::rotate(Transform, glm::radians(Rotation), glm::vec3(0.0, 0.0, 1.0));
	Transform = glm::scale(Transform, glm::vec3(Scale + SpriteTexSize, 0.f));

	SpriteRenderer->SetTransform(Transform);
	SpriteRenderer->SetProjection(Projection);

	SpriteRenderer->Render(
		[&] (SpriteSPType* ShaderProgram){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureID);

			SpriteRenderer->UniformsDescriptor.SetUniform<Texture0Uniform>(0); // 0 corresponds to GL_TEXTURE0

			SpriteRenderer->UniformsDescriptor.SetUniform<TransformUniform>(ShaderProgram->Transform);
			SpriteRenderer->UniformsDescriptor.SetUniform<ProjectionUniform>(ShaderProgram->Projection);
		},
		[&](SpriteSPType* ShaderProgram)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	);
}
