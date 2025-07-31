#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Sprite.hpp"

#include <glm/ext/matrix_transform.hpp>

#include "Logging.h"


Sprite::Sprite(): Renderer(new ShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl")) {
}

Sprite::Sprite(const std::string_view& VertexShader, const std::string_view& FragmentShader): Renderer(new ShaderProgram(VertexShader, FragmentShader)) {
}

Sprite::Sprite(ShaderProgram& OtherShaderProgram): Renderer(&OtherShaderProgram) {
}

Sprite::Sprite(ShaderProgram& OtherShaderProgram, const std::string_view& ImagePath): Renderer(&OtherShaderProgram) {
	LoadImage(ImagePath);
}

Sprite::Sprite(const std::string_view& ImagePath) : Renderer(new ShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl")){
	LoadImage(ImagePath);
}

void Sprite::Init() {

}


void Sprite::LoadImage(const std::string_view& ImagePath) {
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
	if ((ImageWidth * ColorChannels) % 4 == 0) {
		rowAlignment = 4;
	}
	else if ((ImageWidth * ColorChannels) % 2 == 0) {
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
		0, GL_RGB,
		ImageWidth, ImageHeight,
		0, // Legacy arg.
		ColorFormat, GL_UNSIGNED_BYTE, ImageData
	);

	// Reset to default
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	stbi_image_free(ImageData);

	Renderer->TextureID = NewTextureID;
}

void Sprite::Render() {
	glm::mat4 Transform{1.0f};

	Transform = glm::translate(Transform, glm::vec3(Location, 0.f));
	Transform = glm::rotate(Transform, glm::radians(Rotation), glm::vec3(0.0, 0.0, 1.0));
	Transform = glm::scale(Transform, Scale);

	Renderer->SetTranform(Transform);
	Renderer->Render();
}
