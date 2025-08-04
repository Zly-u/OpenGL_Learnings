#pragma once
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

template<typename T>
struct UniformTypeTrait
{
};

template<>
struct UniformTypeTrait<float>
{
		static constexpr GLenum Type = GL_FLOAT;

		static void Set(const GLint Location, const float& Value)
		{
			glUniform1f(Location, Value);
		}
};

template<>
struct UniformTypeTrait<GLuint>
{
		static constexpr GLenum Type = GL_INT;

		static void Set(const GLint Location, const int& Value)
		{
			glUniform1i(Location, Value);
		}
};

template<>
struct UniformTypeTrait<glm::vec2>
{
		static constexpr GLenum Type = GL_FLOAT_VEC2;

		static void Set(const GLint Location, const glm::vec2& Value)
		{
			glUniform2f(Location, Value.x, Value.y);
		}
};

template<>
struct UniformTypeTrait<glm::vec3>
{
		static constexpr GLenum Type = GL_FLOAT_VEC3;

		static void Set(const GLint Location, const glm::vec3& Value)
		{
			glUniform3f(Location, Value.x, Value.y, Value.z);
		}
};

template<>
struct UniformTypeTrait<glm::vec4>
{
		static constexpr GLenum Type = GL_FLOAT_VEC4;

		static void Set(const GLint Location, const glm::vec4& Value)
		{
			glUniform4f(Location, Value.x, Value.y, Value.z, Value.w);
		}
};

template<>
struct UniformTypeTrait<glm::mat4>
{
		static constexpr GLenum Type = GL_FLOAT_MAT4;

		static void Set(const GLint Location, const glm::mat4& Value)
		{
			glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(Value));
		}
};

template<>
struct UniformTypeTrait<glm::mat3>
{
		static constexpr GLenum Type = GL_FLOAT_MAT3;

		static void Set(const GLint Location, const glm::mat3& Value)
		{
			glUniformMatrix3fv(Location, 1, GL_FALSE, glm::value_ptr(Value));
		}
};
