#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Sprite.hpp"
#include "include/Logging.h"
#include <vector>

static const int WindowWith = 1280;
static const int WindowHeight = 720;
int OldWidth, OldHeight;

std::vector<Sprite> Sprites;


ShaderProgram* g_ShaderProgramPtr;



void ProcessInput(GLFWwindow* Window) {
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(Window, true);
	}
}


void Render(GLFWwindow* Window) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_ShaderProgramPtr->Use();
	glBindVertexArray(g_ShaderProgramPtr->VAO);

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShaderProgram g_ShaderProgram{"shaders/vertex.glsl", "shaders/fragment.glsl"};
	g_ShaderProgramPtr = &g_ShaderProgram;

	// Sprites.emplace_back(g_ShaderProgram, "Assets/container.jpg");
	Sprite& Sprite_1 = Sprites.emplace_back(g_ShaderProgram, "Assets/TextureTest.png");
	Sprite_1.Location.x = 1.f;

	Sprite& Sprite_2 = Sprites.emplace_back(g_ShaderProgram, "Assets/TextureTest.png");
	Sprite_2.Location.x = -1.f;

	while(!glfwWindowShouldClose(Window))
	{
		ProcessInput(Window);

		// Render(Window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		g_ShaderProgram.Use();
		glBindVertexArray(g_ShaderProgram.VAO);
		glBindTexture(GL_TEXTURE_2D, g_ShaderProgram.TextureID);

		for (Sprite& Sprite : Sprites) {
			Sprite.Render();
		}

		glfwSwapBuffers(Window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
