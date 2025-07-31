#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.hpp"
#include "include/Logging.h"

static const int WindowWith = 1280;
static const int WindowHeight = 720;
int OldWidth, OldHeight;
uint32_t VAO;

ShaderProgram g_MainShaderProgram;

void ProcessInput(GLFWwindow* Window) {
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(Window, true);
	}
}

void Render(GLFWwindow* Window) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	g_MainShaderProgram.Render(VAO);
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

void PrepareTriangle(uint32_t& VAO) {
	static const float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	uint32_t VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
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

	const ShaderProgram MainShaderProgram{"shaders/vertex.glsl", "shaders/fragment.glsl"};
	g_MainShaderProgram = MainShaderProgram;

	PrepareTriangle(VAO);

	while(!glfwWindowShouldClose(Window))
	{
		glfwPollEvents();
		ProcessInput(Window);

		Render(Window);
	}

	glfwTerminate();

	return 0;
}
