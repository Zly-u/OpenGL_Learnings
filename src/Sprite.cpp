
#include "Sprite.hpp"

#include "AssetsManager.hpp"

#include <GLFW/glfw3.h>


Sprite::Sprite(const std::string_view& ImagePath)
{
	static SpriteSPType* StaticSpriteRenderer = new SpriteSPType{
		"shaders/vertex.glsl",
		"shaders/fragment.glsl",
		SpriteVertices
	};
	ObjectRenderer = StaticSpriteRenderer;

	AssetsManager::LoadImage(ImagePath, TextureID, SpriteTexSize);
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
	Object::Render(Projection);

	ObjectRenderer->Render(
		[&] (ShaderProgramBase* ShaderProgram){
			auto ShaderProgram1 = dynamic_cast<SpriteSPType*>(ShaderProgram);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureID);

			ShaderProgram1->UniformsDescriptor.SetUniform<Texture0Uniform>(0); // 0 corresponds to GL_TEXTURE0

			ShaderProgram1->UniformsDescriptor.SetUniform<TransformUniform>(ShaderProgram1->Transform);
			ShaderProgram1->UniformsDescriptor.SetUniform<ProjectionUniform>(ShaderProgram1->Projection);
		},
		[&](ShaderProgramBase* ShaderProgram)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	);
}
