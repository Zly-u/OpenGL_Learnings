#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.hpp"
#include "include/Logging.h"

static const int WindowWith = 1280;
static const int WindowHeight = 720;
int OldWidth, OldHeight;

uint32_t VBO;
uint32_t VAO;
uint32_t EBO;

ShaderProgram g_MainShaderProgram;

void ProcessInput(GLFWwindow* Window) {
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(Window, true);
	}
}

void Render(GLFWwindow* Window) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	g_MainShaderProgram.Render(VAO, EBO);

	glBindVertexArray(VAO);

	// glDrawArrays(GL_TRIANGLES, 0, 3);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void PrepareTriangle() {
	static const float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

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

void PrepareRectangle() {
	float Vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
	   -0.5f, -0.5f, 0.0f,  // bottom left
	   -0.5f,  0.5f, 0.0f   // top left
   };

	unsigned int Indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

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

	// PrepareTriangle();
	PrepareRectangle();

	while(!glfwWindowShouldClose(Window))
	{
		ProcessInput(Window);

		Render(Window);

		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}
