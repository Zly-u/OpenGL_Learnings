#include "Rendering/Object.hpp"

#include "Rendering/ShaderProgram.hpp"
#include <glm/ext/matrix_transform.hpp>


void Object::Update(const float DeltaTime)
{
}

void Object::Render(const glm::mat4& Projection)
{
	glm::mat4 Transform{1.0f};

	Transform = glm::translate(Transform, glm::vec3(Location.x, Location.y, ZDepth));
	Transform = glm::rotate(Transform, glm::radians(Rotation), glm::vec3(0.0, 0.0, 1.0));
	Transform = glm::scale(Transform, glm::vec3(Scale + SpriteTexSize, 0.f));

	ObjectRenderer->SetTransform(Transform);
	ObjectRenderer->SetProjection(Projection);
}
