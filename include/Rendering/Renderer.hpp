#pragma once

#include "Sprite.hpp"

#include <GLFW/glfw3.h>
#include <vector>

class Renderer
{
	using PositionAttribute = GLMVertexAttribute<0, glm::vec2>;
	using UVAttribute       = GLMVertexAttribute<1, glm::vec2>;

	// TODO: Uniforms syntax like this
	using Texture0Uniform = GLMShaderUniform<"ScreenTexture", GLuint>;

	public:
		struct ScreenVertexData
		{
			glm::vec2 Position;
			glm::vec2 UV;
		};

		using ScreenSP = ShaderProgram<
			ScreenVertexData,
			VertexAttributesList<PositionAttribute, UVAttribute>,
			ShaderUniformsList<Texture0Uniform>
		>;


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
		ScreenSP ScreenRenderer;

		uint32_t FrameBufferObject = 0;
		uint32_t FrameBufferTexture_Color = 0;

		uint32_t RenderBufferObject_DepthAndStencil = 0;
};
