#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Sprite.hpp"
#include "include/Logging.h"
#include <vector>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

static int WindowWidth = 1280;
static int WindowHeight = 720;

glm::mat4 g_Projection;
glm::mat4 g_ViewProjection;

glm::vec2 g_CameraPos{-100.f, 0.f};
glm::mat4 g_CameraView{1.f};

std::vector<Sprite> Sprites;

ShaderProgram* g_ShaderProgramPtr;

void UpdateProjection() {
	g_CameraView = glm::translate(glm::mat4(1.0f), -glm::vec3(g_CameraPos, 0.f));

	g_Projection = glm::ortho(
		0.0f, static_cast<float>(WindowWidth),   // left, right
		static_cast<float>(WindowHeight), 0.0f,	// bottom, top (flipped Y to match top-left origin)
		-1.0f, 1.0f									// near, far
	) * g_CameraView;

	g_ViewProjection = g_Projection * g_CameraView;
}

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
	WindowWidth = NewWidth;
	WindowHeight = NewHeight;

	glViewport(0, 0, NewWidth, NewHeight);

	UpdateProjection();

	// Update Render on resize.
	Render(Window);
}


int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* Window = glfwCreateWindow(WindowWidth, WindowHeight, "OpenGLBitch", nullptr, nullptr);
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
	glfwGetFramebufferSize(Window, &WindowWidth, &WindowHeight);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShaderProgram g_ShaderProgram{"shaders/vertex.glsl", "shaders/fragment.glsl"};
	g_ShaderProgramPtr = &g_ShaderProgram;

	// Sprites.emplace_back(g_ShaderProgram, "Assets/container.jpg");
	Sprite& Sprite_0 = Sprites.emplace_back("Assets/container.jpg", &g_Projection);
	// Sprite_0.Scale *= 1;

	// Sprite& Sprite_1 = Sprites.emplace_back("Assets/TextureTest.png", &g_Projection);
	// Sprite_1.Location.x = 1000.f;
	// Sprite_1.Scale *= 1000;
	//
	// Sprite& Sprite_2 = Sprites.emplace_back("Assets/TextureTest.png", &g_Projection);
	// Sprite_2.Location.x = -1000.f;
	// Sprite_1.Scale *= 1000;
	//
	// Sprite& Sprite_3 = Sprites.emplace_back("Assets/TextureTest.png", &g_Projection);
	// Sprite_3.Location.y = 1000.f;
	// Sprite_1.Scale *= 1000;
	//
	// Sprite& Sprite_4 = Sprites.emplace_back("Assets/TextureTest.png", &g_Projection);
	// Sprite_4.Location.y = -1000.f;
	// Sprite_1.Scale *= 1000;

	while(!glfwWindowShouldClose(Window))
	{
		ProcessInput(Window);

		// TODO: Figure why this doesnt propagate to Sprite and then to Renderer.
		// TODO: Figure why sprites disapear when depth is > 1 or < -1.
		// Sprite_1.ZDepth = glm::sin(glfwGetTime())*10.f;

		UpdateProjection();

		if (GLenum GlError = glGetError(); GlError != GL_NO_ERROR) {
			Log::println("[ERROR::OpenGL] {}", GlError);
		}

		Render(Window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
