#pragma once

#include <glm/matrix.hpp>

class Object
{
	public:
		virtual ~Object() {}

		virtual void Init() = 0;
		virtual void Update(const float DeltaTime) = 0;
		virtual void Render(const glm::mat4& Projection) = 0;
};