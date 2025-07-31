#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.hpp"
#include "Sprite.hpp"
#include "include/Logging.h"
#include <vector>

static const int WindowWith = 1280;
static const int WindowHeight = 720;
int OldWidth, OldHeight;

std::vector<Sprite> Sprites;

void ProcessInput(GLFWwindow* Window) {
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(Window, true);
	}
}


void Render(GLFWwindow* Window) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (Sprite& Sprite : Sprites) {
		Sprite.Render();
	}

	glfwSwapBuffers(Window);
}


void WindowResizedCallback(GLFWwindow* Window, int NewWidth, int NewHeight)
{
	OldWidth = NewWidth;
	OldHeight = NewHeight;

	glViewport(0, 0, NewWidth, NewHeight);

	// Update Render on resize.
	Render(Window);
}


int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* Window = glfwCreateWindow(WindowWith, WindowHeight, "OpenGLBitch", nullptr, nullptr);
	if (Window == nullptr)
	{
		std::printf("Failed to create GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::printf("Failed to initialize GLAD.\n");
		return -1;
	}

	glfwSetWindowSizeCallback(Window, WindowResizedCallback);
	glfwGetFramebufferSize(Window, &OldWidth, &OldHeight);

	glEnable(GL_DEPTH_TEST);

	Sprites.emplace_back();
	// Sprites.emplace_back("shaders/vertex.glsl", "shaders/fragment_red.glsl");

	while(!glfwWindowShouldClose(Window))
	{
		ProcessInput(Window);

		Render(Window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
