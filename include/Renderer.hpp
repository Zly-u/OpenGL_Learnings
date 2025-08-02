#pragma once

#include "Sprite.hpp"
#include "glm/matrix.hpp"

#include <GLFW/glfw3.h>
#include <vector>

class Renderer
{
	public:
		Renderer();
		~Renderer();

		void UpdateProjection(const glm::vec2& WindowSize);
		void Render(GLFWwindow* Window, std::vector<Sprite>& Sprites);


	private:
		glm::mat4 Projection{1.f};
		glm::mat4 ViewProjection{0.f};

		glm::vec2 g_CameraPos{0.f, 0.f};
		glm::mat4 CameraView{1.f};


	private:
		uint32_t FrameBufferObject = 0;
		uint32_t FrameBufferTexture_Color = 0;

		uint32_t RenderBufferObject_DepthAndStencil = 0;


	private:
		ShaderProgram ScreenRenderer;
};
