
#include "SlidingSprite.hpp"

#include "AssetsManager.hpp"

#include <GLFW/glfw3.h>


SlidingSprite::SlidingSprite(const std::string_view& ImagePath)
{
	static SpriteSPType* StaticSpriteRenderer = new SpriteSPType{
		"shaders/vertex.glsl",
		"shaders/fragment.glsl",
		SpriteVertices
	};
	ObjectRenderer = StaticSpriteRenderer;

	AssetsManager::LoadImage(ImagePath, TextureID, SpriteTexSize);
}

SlidingSprite::~SlidingSprite()
{

}


// -----------------------------------------------------------------------------------------------------------


void SlidingSprite::Init()
{

}

static float Timer = 0.f;
void SlidingSprite::Update(const float DeltaTime)
{
	Timer += DeltaTime;
}


void SlidingSprite::Render(const glm::mat4& Projection) {
	Object::Render(Projection);

	ObjectRenderer->Render(
		[&] (ShaderProgramBase* ShaderProgram){
			auto ShaderProgram1 = dynamic_cast<SpriteSPType*>(ShaderProgram);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureID);

			const float skewy = 0.5 + glm::sin(Timer) * 0.5;
			const std::array<glm::vec2, 4> NewUV = {
				glm::vec2
				{ 0.0f + skewy, 0.0f },
				{ 1.0f + skewy, 0.0f },
				{ 1.0f + skewy, 1.0f },
				{ 0.0f + skewy, 1.0f }
			};
			ShaderProgram1->VertexAttributesHandler.SetAttributeBufferData<UVAttribute>(NewUV);

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
