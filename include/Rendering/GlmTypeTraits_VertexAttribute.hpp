#pragma once

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

template<typename T>
struct GLMTypeTrait { };

template<>
struct GLMTypeTrait<glm::vec2> {
	using ComponentType = float;

    static constexpr GLint Size = 2;
    static constexpr GLenum EnumType = GL_FLOAT;
};

template<>
struct GLMTypeTrait<glm::vec3> {
    using ComponentType = float;

    static constexpr GLint Size = 3;
    static constexpr GLenum EnumType = GL_FLOAT;
};

template<>
struct GLMTypeTrait<glm::vec4> {
    using ComponentType = float;

    static constexpr GLint Size = 4;
    static constexpr GLenum EnumType = GL_FLOAT;
};

template<>
struct GLMTypeTrait<glm::ivec2> {
    using ComponentType = int;

    static constexpr GLint Size = 2;
    static constexpr GLenum EnumType = GL_INT;
};

template<>
struct GLMTypeTrait<glm::ivec3> {
    using ComponentType = int;

    static constexpr GLint Size = 3;
    static constexpr GLenum EnumType = GL_INT;
};

template<>
struct GLMTypeTrait<glm::ivec4> {
    using ComponentType = int;

    static constexpr GLint Size = 4;
    static constexpr GLenum EnumType = GL_INT;
};

template<>
struct GLMTypeTrait<glm::uvec2> {
    using ComponentType = unsigned int;

    static constexpr GLint Size = 2;
    static constexpr GLenum EnumType = GL_UNSIGNED_INT;
};

template<>
struct GLMTypeTrait<glm::uvec3> {
    using ComponentType = unsigned int;

    static constexpr GLint Size = 3;
    static constexpr GLenum EnumType = GL_UNSIGNED_INT;
};

template<>
struct GLMTypeTrait<glm::uvec4> {
    using ComponentType = unsigned int;

    static constexpr GLint Size = 4;
    static constexpr GLenum EnumType = GL_UNSIGNED_INT;
};

// Double precision vectors
template<>
struct GLMTypeTrait<glm::dvec2> {
    using ComponentType = double;

    static constexpr GLint Size = 2;
    static constexpr GLenum EnumType = GL_DOUBLE;
};

template<>
struct GLMTypeTrait<glm::dvec3> {
    using ComponentType = double;

    static constexpr GLint Size = 3;
    static constexpr GLenum EnumType = GL_DOUBLE;
};

template<>
struct GLMTypeTrait<glm::dvec4> {
    using ComponentType = double;

    static constexpr GLint Size = 4;
    static constexpr GLenum EnumType = GL_DOUBLE;
};