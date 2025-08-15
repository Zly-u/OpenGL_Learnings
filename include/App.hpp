#pragma once

#include "Delegates.h"
#include "Rendering/Renderer.hpp"

#include <GLFW/glfw3.h>
#include <expected>
#include <memory>
#include <string_view>

class App {
	protected:
		App();

	public:
		~App();
		App(const App&)       = delete;
		App(const App&&)      = delete;
		App& operator=(App&&) = delete;

		static App& Get();


	public:
		Delegate<> OnWindowResized;


	public:
		std::expected<bool, std::string_view> Init();
		void PostInit();

		void WindowResizedCallback(int NewWidth, int NewHeight);
		const glm::vec2& GetWindowSize() const;

		void ProcessInput();
		void Update(const float DeltaTime);
		void GameLoop();

		void SortObjects();

		template<typename RenderableType, typename... Args>
		RenderableType* CreateRenderable(Args... CtorArgs)
		{
			auto* NewObject = new RenderableType(std::forward<Args>(CtorArgs)...);
			Objects.emplace_back(NewObject);
			return NewObject;
		}


	private:
		__forceinline void UpdateDeltaTime();

	public:
		void SetWindowTitle(const std::string& Title) const;

	private:
		// For setting up the base resolution for the monitor.
		glm::vec2 BaseWindowSize{1280.f, 720.f};

		// For the Viewport.
		glm::vec2 WindowSize{1280.f, 720.f};

		// NOTE: Temporarily here cuz we have no Scenes or Levels to store them.
		std::vector<Object*> Objects;


	private:
		GLFWwindow* Window = nullptr;
		std::unique_ptr<Renderer> TheRenderer;

		float DeltaTime = 0.f;
		float PreviousFrameTime = 0.f;
};
