#include "Renderer.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}


void Renderer::UpdateProjection(const glm::vec2& WindowSize)
{
	CameraView = glm::translate(glm::mat4(1.0f), -glm::vec3(g_CameraPos, 0.f));

	Projection = glm::ortho(
		0.0f, WindowSize.x,	// left, right
		WindowSize.y, 0.0f,	// bottom, top (flipped Y to match top-left origin)
		-1000.0f, 1000.0f   // near, far
	)
	* CameraView;

	ViewProjection = Projection * CameraView;
}


void Renderer::Render(GLFWwindow* Window, std::vector<Sprite>& Sprites)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (Sprite& Sprite : Sprites)
	{
		Sprite.Render(Projection);
	}

	glfwSwapBuffers(Window);
}
