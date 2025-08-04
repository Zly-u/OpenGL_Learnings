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
		WindowSize.x,
		WindowSize.y,
		"OpenGL Learnings",
		nullptr,
		nullptr
	);
	if (Window == nullptr)
	{
		glfwTerminate();
		return std::unexpected("Failed to create GLFW window.");
	}
	glfwMakeContextCurrent(Window);
	glfwSetWindowSizeCallback(Window, WindowResizedCallbackWrapper);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		return std::unexpected("Failed to initialize GLAD.");
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	TheRenderer = std::make_unique<Renderer>();

	return true;
}


void App::PostInit()
{
	Sprites.reserve(100);

	Sprite& Sprite_0  = Sprites.emplace_back("Assets/AmyAAAA.png");
	Sprite_0.Name     = "Amy1";
	Sprite_0.Location = glm::vec2(WindowSize.x / 1.5f, WindowSize.y / 2.f);

	Sprite& Sprite_1  = Sprites.emplace_back("Assets/AmyM.png");
	Sprite_1.Name     = "Amy2";
	Sprite_1.Location = glm::vec2(WindowSize.x / 3.f, WindowSize.y / 2.f);
}


void App::WindowResizedCallback(const int NewWidth, const int NewHeight)
{
	WindowSize.x = NewWidth;
	WindowSize.y = NewHeight;

	glViewport(0, 0, NewWidth, NewHeight);

	TheRenderer->UpdateProjection(BaseWindowSize);

	OnWindowResized.Broadcast();

	// Update Render on resize.
	TheRenderer->Render(Window, Sprites);
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

		TheRenderer->UpdateProjection(BaseWindowSize);
		TheRenderer->Render(Window, Sprites);

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
