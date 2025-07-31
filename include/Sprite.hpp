#pragma once

#include <array>

#include "ShaderProgram.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

struct TextureData
{
	std::array<glm::vec2, 4> UVs;
	std::array<glm::vec4, 4> VertexColors;
};

class Sprite {
	public:
		Sprite();
		Sprite(const std::string_view& VertexShader, const std::string_view& FragmentShader);

		void Init();
		void Render();

	private:
		ShaderProgram Renderer;
		// TODO Texture;
};
