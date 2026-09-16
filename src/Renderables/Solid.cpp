#include "Renderables/Solid.hpp"

#include "AssetsManager.hpp"

#include <GLFW/glfw3.h>


Solid::Solid()
{
	static ShaderProgram_Solid* StaticSpriteRenderer = new ShaderProgram_Solid{
		"shaders/vertex.glsl",
		"shaders/fragment_solid.glsl",
		SolidVertices
	};
	ObjectRenderer = StaticSpriteRenderer;
}

Solid::~Solid()
{

}


// -----------------------------------------------------------------------------------------------------------


void Solid::Init()
{

}

void Solid::Update(const float DeltaTime)
{

}


void Solid::Render(const glm::mat4& Projection) {
	Object::Render(Projection);

	ObjectRenderer->Render(
		[&] (ShaderProgramBase* ShaderProgram)
		{
			const auto ShaderProgram1 = dynamic_cast<ShaderProgram_Solid*>(ShaderProgram);

			ShaderProgram1->UniformsDescriptor.SetUniform<TransformUniform>(ShaderProgram1->Transform);
			ShaderProgram1->UniformsDescriptor.SetUniform<ProjectionUniform>(ShaderProgram1->Projection);
		},
		[&](ShaderProgramBase* ShaderProgram)
		{
		}
	);
}
