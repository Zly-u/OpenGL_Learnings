#include "App.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Logging.h"
#include "SpritePixelization.hpp"

#include <chrono>

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	TheRenderer = std::make_unique<Renderer>();

	return true;
}


void App::PostInit()
{
	Objects.reserve(20000);

	auto ChronoStart = std::chrono::high_resolution_clock::now();

	for (int Index = 0; Index < 1000; ++Index)
	{
		Sprite* NewSprite = CreateRenderable<Sprite>("Assets/panel-border-005.png");

		NewSprite->Location = glm::vec2(
			Random::Normal() * WindowSize.x,
			Random::Normal() * WindowSize.y
		);
		NewSprite->ZDepth = -Random::Normal() * 100;
		NewSprite->Scale = glm::vec2(1.f) * Random::RangeFloat(0.1f, 0.5f);
		NewSprite->Rotation = Random::RangeFloat(-1.f, 1.f) * 180.f;
	}

	Sprite* Sprite_0   = CreateRenderable<Sprite>("Assets/AmyAAAA.png");
	Sprite_0->Name     = "Amy1";
	Sprite_0->Location = glm::vec2(WindowSize.x / 1.5f, WindowSize.y / 2.f);
	Sprite_0->ZDepth = 10.f;

	SpritePixelization* Sprite_1 = CreateRenderable<SpritePixelization>("Assets/AmyM.png");
	Sprite_1->Name     = "Amy2";
	Sprite_1->Location = glm::vec2(WindowSize.x / 3.f, WindowSize.y / 2.f);

	SortObjects();

	auto ChronoEnd = std::chrono::high_resolution_clock::now();
	auto Duration = std::chrono::duration_cast<std::chrono::microseconds>(ChronoEnd - ChronoStart);
	Log::println("Objects creation took: {:.2f} sec", float(float(Duration.count()) / 1000000.f));
}


void App::WindowResizedCallback(const int NewWidth, const int NewHeight)
{
	WindowSize.x = NewWidth;
	WindowSize.y = NewHeight;

	glViewport(0, 0, NewWidth, NewHeight);

	TheRenderer->UpdateProjection(BaseWindowSize);

	OnWindowResized.Broadcast();

	// Update Render on resize.
	TheRenderer->Render(Window, Objects);
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
	for (Object* Object : Objects)
	{
		Object->Update(DeltaTime);
	}
}


float FpsTimer = 0.5;
float FpsCurrentTime = 0.f;
void App::GameLoop()
{
	while (!glfwWindowShouldClose(Window))
	{
		UpdateDeltaTime();

		FpsCurrentTime -= DeltaTime;
		if (FpsCurrentTime <= 0.f)
		{
			SetWindowTitle(std::format("FPS: {:.2f}", 1.f / DeltaTime));
			FpsCurrentTime = FpsTimer;
		}

		ProcessInput();

		Update(DeltaTime);

		TheRenderer->UpdateProjection(BaseWindowSize);
		TheRenderer->Render(Window, Objects);

		if (GLenum GlError = glGetError(); GlError != GL_NO_ERROR)
		{
			Log::println("[ERROR::OpenGL] {}", GlError);
		}

		glfwPollEvents();
	}
}

void App::SortObjects()
{
	std::ranges::sort(
		Objects,
		[](const Object* A, const Object* B)
		{
			return A->ZDepth < B->ZDepth;
		}
	);
}

void App::UpdateDeltaTime()
{
	const float Ticks = glfwGetTime();
	DeltaTime         = Ticks - PreviousFrameTime;
	PreviousFrameTime = Ticks;
}

void App::SetWindowTitle(const std::string& Title) const
{
	glfwSetWindowTitle(Window, Title.c_str());
}
