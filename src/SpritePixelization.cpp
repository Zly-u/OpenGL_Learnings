#include "SpritePixelization.hpp"

#include "AssetsManager.hpp"

#include <GLFW/glfw3.h>


SpritePixelization::SpritePixelization(const std::string_view& ImagePath)
{
	static SpritePxSPType* StaticSpriteRenderer = new SpritePxSPType{
		"shaders/vertex.glsl",
		"shaders/fragment_pixelating.glsl",
		SpriteVertices
	};
	ObjectRenderer = StaticSpriteRenderer;

	AssetsManager::LoadImage(ImagePath, TextureID, SpriteTexSize);
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


void SpritePixelization::Render(const glm::mat4& Projection)
{
	Object::Render(Projection);

	ObjectRenderer->Render(
		[&] (ShaderProgramBase* ShaderProgram){
			auto SpriteRenderer = dynamic_cast<SpritePxSPType*>(ShaderProgram);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureID);

			SpriteRenderer->UniformsDescriptor.SetUniform<Texture0Uniform>(0); // 0 corresponds to GL_TEXTURE0

			SpriteRenderer->UniformsDescriptor.SetUniform<TransformUniform>(SpriteRenderer->Transform);
			SpriteRenderer->UniformsDescriptor.SetUniform<ProjectionUniform>(SpriteRenderer->Projection);

			SpriteRenderer->UniformsDescriptor.SetUniform<PixelationUniform>(PixelationLevel);
		},
		[&](ShaderProgramBase* ShaderProgram)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	);
}