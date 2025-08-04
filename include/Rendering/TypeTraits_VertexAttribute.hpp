#pragma once

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

template<typename T>
struct VertexAttributeTypeTrait { };

template<>
struct VertexAttributeTypeTrait<glm::vec2> {
	using ComponentType = float;

    static constexpr GLint Size = 2;
    static constexpr GLenum EnumType = GL_FLOAT;
};

template<>
struct VertexAttributeTypeTrait<glm::vec3> {
    using ComponentType = float;

    static constexpr GLint Size = 3;
    static constexpr GLenum EnumType = GL_FLOAT;
};

template<>
struct VertexAttributeTypeTrait<glm::vec4> {
    using ComponentType = float;

    static constexpr GLint Size = 4;
    static constexpr GLenum EnumType = GL_FLOAT;
};

template<>
struct VertexAttributeTypeTrait<glm::ivec2> {
    using ComponentType = int;

    static constexpr GLint Size = 2;
    static constexpr GLenum EnumType = GL_INT;
};

template<>
struct VertexAttributeTypeTrait<glm::ivec3> {
    using ComponentType = int;

    static constexpr GLint Size = 3;
    static constexpr GLenum EnumType = GL_INT;
};

template<>
struct VertexAttributeTypeTrait<glm::ivec4> {
    using ComponentType = int;

    static constexpr GLint Size = 4;
    static constexpr GLenum EnumType = GL_INT;
};

template<>
struct VertexAttributeTypeTrait<glm::uvec2> {
    using ComponentType = unsigned int;

    static constexpr GLint Size = 2;
    static constexpr GLenum EnumType = GL_UNSIGNED_INT;
};

template<>
struct VertexAttributeTypeTrait<glm::uvec3> {
    using ComponentType = unsigned int;

    static constexpr GLint Size = 3;
    static constexpr GLenum EnumType = GL_UNSIGNED_INT;
};

template<>
struct VertexAttributeTypeTrait<glm::uvec4> {
    using ComponentType = unsigned int;

    static constexpr GLint Size = 4;
    static constexpr GLenum EnumType = GL_UNSIGNED_INT;
};

// Double precision vectors
template<>
struct VertexAttributeTypeTrait<glm::dvec2> {
    using ComponentType = double;

    static constexpr GLint Size = 2;
    static constexpr GLenum EnumType = GL_DOUBLE;
};

template<>
struct VertexAttributeTypeTrait<glm::dvec3> {
    using ComponentType = double;

    static constexpr GLint Size = 3;
    static constexpr GLenum EnumType = GL_DOUBLE;
};

template<>
struct VertexAttributeTypeTrait<glm::dvec4> {
    using ComponentType = double;

    static constexpr GLint Size = 4;
    static constexpr GLenum EnumType = GL_DOUBLE;
};