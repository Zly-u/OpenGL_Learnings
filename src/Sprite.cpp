#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Sprite.hpp"
#include "Logging.h"


Sprite::Sprite(): Renderer("shaders/vertex.glsl", "shaders/fragment.glsl") {
	Init();
}

Sprite::Sprite(const std::string_view& ImagePath) : Renderer("shaders/vertex.glsl", "shaders/fragment.glsl"){
	Init();

	int ImageWidth, ImageHeight, ColorChannels;
	unsigned char * ImageData = stbi_load(ImagePath.data(), &ImageWidth, &ImageHeight, &ColorChannels, 0);
	if (!ImageData)
	{
		Log::println("[ERROR]: Failed to load an image: {}", ImagePath);
		return;
	}

	// Determine format based on channels
	GLenum ColorFormat = GL_RGB;
	if (ColorChannels == 1) {
		ColorFormat = GL_RED;
	}
	else if (ColorChannels == 3) {
		ColorFormat = GL_RGB;
	}
	else if (ColorChannels == 4) {
		ColorFormat = GL_RGBA;
	}

	int rowAlignment = 1;
	if ((ImageWidth * ColorChannels) % 4 == 0) rowAlignment = 4;
	else if ((ImageWidth * ColorChannels) % 2 == 0) rowAlignment = 2;
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
		0, GL_RGB,
		ImageWidth, ImageHeight,
		0, // Legacy arg.
		ColorFormat, GL_UNSIGNED_BYTE, ImageData
	);

	// Reset to default
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	stbi_image_free(ImageData);

	Renderer.TextureID = NewTextureID;
}

Sprite::Sprite(const std::string_view& VertexShader, const std::string_view& FragmentShader): Renderer(VertexShader, FragmentShader) {
	Init();
}

void Sprite::Init() {

}

void Sprite::Render() {
	Renderer.Render();
}
