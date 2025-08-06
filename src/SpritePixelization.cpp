#include "SpritePixelization.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>


SpritePixelization::SpritePixelization(const std::string_view& ImagePath)
{
	static SpritePxSPType StaticSpriteRenderer{
		"shaders/vertex.glsl",
		"shaders/fragment_pixelating.glsl",
		SpriteVertices
	};
	SpriteRenderer = &StaticSpriteRenderer;

	LoadImage(ImagePath);
}

SpritePixelization::~SpritePixelization()
{

}


// -----------------------------------------------------------------------------------------------------------


void SpritePixelization::Init()
{

}

float Timer = 0.f;
void SpritePixelization::Update(const float DeltaTime)
{
	Timer += DeltaTime;
	PixelationLevel = 5.f + glm::cos(Timer * 2.f) * 5.f;
}


void SpritePixelization::Render(const glm::mat4& Projection) {
	glm::mat4 Transform{1.0f};

	Transform = glm::translate(Transform, glm::vec3(Location.x, Location.y, ZDepth));
	Transform = glm::rotate(Transform, glm::radians(Rotation), glm::vec3(0.0, 0.0, 1.0));
	Transform = glm::scale(Transform, glm::vec3(Scale + SpriteTexSize, 0.f));

	SpriteRenderer->SetTransform(Transform);
	SpriteRenderer->SetProjection(Projection);

	SpriteRenderer->Render(
		[&] (SpritePxSPType* ShaderProgram){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureID);

			SpriteRenderer->UniformsDescriptor.SetUniform<Texture0Uniform>(0); // 0 corresponds to GL_TEXTURE0

			SpriteRenderer->UniformsDescriptor.SetUniform<TransformUniform>(ShaderProgram->Transform);
			SpriteRenderer->UniformsDescriptor.SetUniform<ProjectionUniform>(ShaderProgram->Projection);

			SpriteRenderer->UniformsDescriptor.SetUniform<PixelationUniform>(PixelationLevel);
		},
		[&](SpritePxSPType* ShaderProgram)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	);
}