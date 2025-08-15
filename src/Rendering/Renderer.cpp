#include "Rendering/Renderer.hpp"

#include "App.hpp"
#include "Logging.h"
#include "Rendering/Object.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

// clang-format off
// Different positions and UVs are flipped due to how Framebuffer stores data.
static const std::array<Renderer::ScreenVertexData, 4> ScreenVertices = {
	Renderer::ScreenVertexData
	{
		.Position = { -1.f, 1.f },
		.UV       = { 0.0f, 1.0f }
	},
	{
		.Position = { 1.f, 1.f },
		.UV       = { 1.0f, 1.0f }
	},
	{
		.Position = { 1.f, -1.f },
		.UV       = { 1.0f, 0.0f }
	},
	{
		.Position = { -1.f, -1.f },
		.UV       = { 0.0f, 0.0f }
	}
};
// clang-format on

Renderer::Renderer()
	: ScreenRenderer(
		"shaders/vertex_screen.glsl",
		"shaders/fragment_screen.glsl",
		ScreenVertices
	)
{
	InitializeBuffers();

	App::Get().OnWindowResized.Bind(
		[&]
		{
			Cleanup();
			InitializeBuffers();
		}
	);
}


Renderer::~Renderer()
{
	Cleanup();
}


void Renderer::InitializeBuffers()
{
	const glm::vec2& WindowSize = App::Get().GetWindowSize();

	// -----------------------------------------------------------------------------------

	// Frame Buffer Object

	glGenFramebuffers(1, &FrameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObject);

	glGenTextures(1, &FrameBufferTexture_Color);
	glBindTexture(GL_TEXTURE_2D, FrameBufferTexture_Color);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB,
		WindowSize.x, WindowSize.y,
		0,
		GL_RGB, GL_UNSIGNED_BYTE,
		nullptr
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		FrameBufferTexture_Color, 0
	);

	// -----------------------------------------------------------------------------------

	// Render Buffer Object

	glGenRenderbuffers(1, &RenderBufferObject_DepthAndStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, RenderBufferObject_DepthAndStencil);
	glRenderbufferStorage(
		GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
		WindowSize.x, WindowSize.y
	);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
		RenderBufferObject_DepthAndStencil
	);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Log::println("[ERROR] Framebuffer is incomplete.");
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// -----------------------------------------------------------------------------------
}


void Renderer::Cleanup()
{
	glDeleteRenderbuffers(1, &RenderBufferObject_DepthAndStencil);
	glDeleteFramebuffers(1, &FrameBufferObject);
}


void Renderer::UpdateProjection(const glm::vec2& WindowSize)
{
	CameraView = glm::translate(glm::mat4(1.0f), -glm::vec3(g_CameraPos, 0.f));

	Projection = glm::ortho(
		0.0f, WindowSize.x,	// left, right
		WindowSize.y, 0.0f,	// bottom, top (flipped Y to match top-left origin)
		-1000.0f, 1000.0f   // near, far
	);

	ViewProjection = Projection * CameraView;
}


void Renderer::Render(GLFWwindow* Window, const std::vector<Object*>& Objects)
{
	// First pass - render scene
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObject);
		// glEnable(GL_DEPTH_TEST);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		for (Object* Sprite : Objects)
		{
			Sprite->Render(Projection);
		}
	}

	glBindVertexArray(0);

	// Second pass - display rendered scene on a screen polygon
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		ScreenRenderer.Render(
			[&](ShaderProgramBase* ShaderProgram)
			{
				auto* ScreenRenderer1 = dynamic_cast<ScreenSP*>(ShaderProgram);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, FrameBufferTexture_Color);
				ScreenRenderer1->UniformsDescriptor.SetUniform<TextureScreenUniform>(0); // 0 corresponds to GL_TEXTURE0
			},
			[&](ShaderProgramBase* ShaderProgram)
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		);
	}

	glfwSwapBuffers(Window);
}
