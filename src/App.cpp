#include "App.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Logging.h"

// ---------------------------------------------------------------------------------------------------------------------

void WindowResizedCallbackWrapper(
	GLFWwindow* Window,
	const int   NewWidth,
	const int   NewHeight
)
{
	App::Get().WindowResizedCallback(NewWidth, NewHeight);
}

// ---------------------------------------------------------------------------------------------------------------------

App::App()
{
}

App::~App()
{
	glfwTerminate();
}

// ---------------------------------------------------------------------------------------------------------------------

App& App::Get()
{
	static App Instance;
	return Instance;
}

// ---------------------------------------------------------------------------------------------------------------------

std::expected<bool, std::string_view> App::Init()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window = glfwCreateWindow(
		WindowSize.x, WindowSize.y,
		"OpenGL Learnings",
		nullptr, nullptr
	);
	if (Window == nullptr)
	{
		glfwTerminate();
		return std::unexpected("Failed to create GLFW window.");
	}
	glfwMakeContextCurrent(Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		return std::unexpected("Failed to initialize GLAD.");
	}

	glfwSetWindowSizeCallback(Window, WindowResizedCallbackWrapper);
	// glfwGetFramebufferSize(Window, &WindowSize.x, &WindowSize.y);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}


void App::PostInit()
{
	Sprites.reserve(100);

	Sprite& Sprite_0 = Sprites.emplace_back("Assets/container.jpg");
	Sprite_0.Location = glm::vec2(WindowSize.x / 1.5f, WindowSize.y / 2.f);
	Sprite_0.Name = "Box";

	Sprite& Sprite_1 = Sprites.emplace_back("Assets/TextureTest.png");
	Sprite_1.Location = glm::vec2(WindowSize.x / 3.f, WindowSize.y / 2.f);
	Sprite_1.Name = "Guy";

	// Sprites.emplace_back(g_ShaderProgram, "Assets/container.jpg");
	// Sprite& Sprite_0 = Sprites.emplace_back("Assets/container.jpg", &g_Projection);
	// Sprite_0.Location = glm::vec2(WindowWidth / 1.5f, WindowHeight / 2.f);
	// Sprite_0.Name = "Box";
	//
	// Sprite& Sprite_1 = Sprites.emplace_back("Assets/TextureTest.png", &g_Projection);
	// Sprite_1.Location = glm::vec2(WindowWidth / 3.f, WindowHeight / 2.f);
	// Sprite_1.Name = "Guy";

	// Sprite& Sprite_2 = Sprites.emplace_back("Assets/TextureTest.png", &g_Projection);
	// Sprite& Sprite_3 = Sprites.emplace_back("Assets/TextureTest.png", &g_Projection);
	// Sprite& Sprite_4 = Sprites.emplace_back("Assets/TextureTest.png", &g_Projection);
}


void App::WindowResizedCallback(const int NewWidth, const int NewHeight)
{
	WindowSize.x = NewWidth;
	WindowSize.y = NewHeight;

	glViewport(0, 0, NewWidth, NewHeight);

	TheRenderer.UpdateProjection(BaseWindowSize);

	// Update Render on resize.
	TheRenderer.Render(Window, Sprites);
}

const glm::vec2& App::GetWindowSize() const
{
	return WindowSize;
}

void App::ProcessInput()
{
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Window, true);
	}
}

void App::Update(const float DeltaTime)
{
	for (Sprite& Sprite : Sprites)
	{
		Sprite.Update(DeltaTime);
	}
}

void App::GameLoop()
{
	while (!glfwWindowShouldClose(Window))
	{
		UpdateDeltaTime();

		ProcessInput();

		Update(DeltaTime);

		TheRenderer.UpdateProjection(BaseWindowSize);
		TheRenderer.Render(Window, Sprites);

		if (GLenum GlError = glGetError(); GlError != GL_NO_ERROR)
		{
			Log::println("[ERROR::OpenGL] {}", GlError);
		}

		glfwPollEvents();
	}
}

void App::UpdateDeltaTime()
{
	const float Ticks = glfwGetTime();
	DeltaTime         = Ticks - PreviousFrameTime;
	PreviousFrameTime = Ticks;
}
