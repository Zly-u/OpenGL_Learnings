#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Sprite.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

#include "Logging.h"
#include "Random.h"

// clang-format off
static const std::array<Sprite::SpriteVertexData, 4> SpriteVertices = {
	Sprite::SpriteVertexData
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

Sprite::Sprite(const std::string_view& ImagePath)
	: SpriteRenderer(new SpriteSP("shaders/vertex.glsl", "shaders/fragment.glsl", SpriteVertices))
{
	LoadImage(ImagePath);
	SpriteRenderer->UniformsDescriptor.SetGraphicsUpdatingFunction(
		[&]
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, SpriteRenderer->TextureID);

			SpriteRenderer->UniformsDescriptor.SetUniform<Texture0Uniform>(0); // 0 corresponds to GL_TEXTURE0

			SpriteRenderer->UniformsDescriptor.SetUniform<TransformUniform>(SpriteRenderer->Transform);
			SpriteRenderer->UniformsDescriptor.SetUniform<ProjectionUniform>(SpriteRenderer->Projection);
		}
	);
}


void Sprite::Init() {

}


void Sprite::LoadImage(const std::string_view& ImagePath)
{
	stbi_set_flip_vertically_on_load(true);

	int            ImageWidth, ImageHeight, ColorChannels;
	unsigned char* ImageData =
		stbi_load(ImagePath.data(), &ImageWidth, &ImageHeight, &ColorChannels, 0);
	if (!ImageData)
	{
		Log::println("[ERROR]: Failed to load an image: {}", ImagePath);
		return;
	}

	// Determine format based on channels
	GLenum ColorFormat = GL_RGB;
	if (ColorChannels == 1)
	{
		ColorFormat = GL_RED;
	}
	else if (ColorChannels == 3)
	{
		ColorFormat = GL_RGB;
	}
	else if (ColorChannels == 4)
	{
		ColorFormat = GL_RGBA;
	}

	int rowAlignment = 1;
	if ((ImageWidth * ColorChannels) % 4 == 0)
	{
		rowAlignment = 4;
	}
	else if ((ImageWidth * ColorChannels) % 2 == 0)
	{
		rowAlignment = 2;
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, rowAlignment);

	uint32_t NewTextureID;
	glGenTextures(1, &NewTextureID);
	glBindTexture(GL_TEXTURE_2D, NewTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		ImageWidth,
		ImageHeight,
		0,    // Legacy arg.
		ColorFormat,
		GL_UNSIGNED_BYTE,
		ImageData
	);

	// Reset to default
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	stbi_image_free(ImageData);

	SpriteRenderer->TextureID = NewTextureID;
	SpriteTexSize             = glm::vec2(ImageWidth, ImageHeight);
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

	SpriteRenderer->Render();
}
