#include "AssetsManager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Logging.h"

#include "stb_image.h"
#include <glad/glad.h>


void AssetsManager::LoadImage(
	const std::string_view& Asset,
	uint32_t&               OutTextureID,
	glm::vec2&              OutImageSize
)
{
	if (auto FoundTexture = TextureIDs.find(Asset); FoundTexture != TextureIDs.end())
	{
		auto& [ID, Size] = FoundTexture->second;
		OutTextureID     = ID;
		OutImageSize     = Size;

		return;
	}

	stbi_set_flip_vertically_on_load(true);

	int            ImageWidth, ImageHeight, ColorChannels;
	unsigned char* ImageData = stbi_load(Asset.data(), &ImageWidth, &ImageHeight, &ColorChannels, 0);
	if (!ImageData)
	{
		Log::println("[ERROR]: Failed to load an image: {}", Asset);
		return;
	}

	OutImageSize = glm::vec2(ImageWidth, ImageHeight);

	// Determine format based on channels
	GLenum ColorFormat = GL_RGB;
	GLenum InternalFormat = GL_RGB8;
	if (ColorChannels == 3)
	{
		ColorFormat = GL_RGB;
		InternalFormat = GL_RGB8;
	}
	else if (ColorChannels == 4)
	{
		ColorFormat = GL_RGBA;
		InternalFormat = GL_RGBA8;
	}

	Log::println("[INFO]: Loaded an image: {}, with colors of firs pixel: {}, {}, {}, {}", Asset, ImageData[0], ImageData[1], ImageData[2], ImageData[3]);

	int rowAlignment = 1;
	const int BytesPerRow = ImageWidth * ColorChannels;
	if (BytesPerRow % 4 == 0)
	{
		rowAlignment = 4;
	}
	else if (BytesPerRow % 2 == 0)
	{
		rowAlignment = 2;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, rowAlignment);

	glGenTextures(1, &OutTextureID);
	glBindTexture(GL_TEXTURE_2D, OutTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		InternalFormat,
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

	TextureIDs.emplace(Asset, std::make_pair(OutTextureID, OutImageSize));

	glBindTexture(GL_TEXTURE_2D, 0);
}

