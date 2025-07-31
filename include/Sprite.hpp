#pragma once

#include <array>

#include "ShaderProgram.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/matrix.hpp"

struct TextureData
{
	// TODO: Utilize this?
	std::array<glm::vec2, 4> UVs;
	std::array<glm::vec4, 4> VertexColors;
};

class Sprite {
	public:
		Sprite();
		Sprite(ShaderProgram& OtherShaderProgram, const std::string_view& ImagePath);
		Sprite(const std::string_view& ImagePath);
		Sprite(const std::string_view& VertexShader, const std::string_view& FragmentShader);
		Sprite(ShaderProgram& OtherShaderProgram);

		void Init();
		void LoadImage(const std::string_view& ImagePath);
		void Render();


	public:
		glm::vec2 Location{0.f};
		float ZDepth = 0.f;
		float Rotation = 0.f;
		glm::vec3 Scale{1.f};


	private:
		ShaderProgram* Renderer;
};
