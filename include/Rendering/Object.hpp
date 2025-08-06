#pragma once

#include <glm/matrix.hpp>
#include <string>
#include <string_view>

class Object
{
	public:
		virtual ~Object() {}

		virtual void Init() = 0;
		virtual void Update(const float DeltaTime) = 0;
		virtual void Render(const glm::mat4& Projection) = 0;


		void LoadImage(const std::string_view& ImagePath);

	public:
		std::string Name;
		glm::vec2   SpriteTexSize{ 1.f };

		glm::vec2 Location{ 0.f };
		float     Rotation = 0.f;
		glm::vec2 Scale{ 1.f };

		float ZDepth = 0.f;


	protected:
		uint32_t TextureID = 0;
};