#pragma once

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

template<typename T>
struct GLMTypeTrait { };

template<>
struct GLMTypeTrait<glm::vec2> {
    static constexpr GLint Size = 2;
    static constexpr GLenum Type = GL_FLOAT;
    using ComponentType = float;
};

template<>
struct GLMTypeTrait<glm::vec3> {
    static constexpr GLint Size = 3;
    static constexpr GLenum Type = GL_FLOAT;
    using ComponentType = float;
};

template<>
struct GLMTypeTrait<glm::vec4> {
    static constexpr GLint Size = 4;
    static constexpr GLenum Type = GL_FLOAT;
    using ComponentType = float;
};

template<>
struct GLMTypeTrait<glm::ivec2> {
    static constexpr GLint Size = 2;
    static constexpr GLenum Type = GL_INT;
    using ComponentType = int;
};

template<>
struct GLMTypeTrait<glm::ivec3> {
    static constexpr GLint Size = 3;
    static constexpr GLenum Type = GL_INT;
    using ComponentType = int;
};

template<>
struct GLMTypeTrait<glm::ivec4> {
    static constexpr GLint Size = 4;
    static constexpr GLenum Type = GL_INT;
    using ComponentType = int;
};

template<>
struct GLMTypeTrait<glm::uvec2> {
    static constexpr GLint Size = 2;
    static constexpr GLenum Type = GL_UNSIGNED_INT;
    using ComponentType = unsigned int;
};

template<>
struct GLMTypeTrait<glm::uvec3> {
    static constexpr GLint Size = 3;
    static constexpr GLenum Type = GL_UNSIGNED_INT;
    using ComponentType = unsigned int;
};

template<>
struct GLMTypeTrait<glm::uvec4> {
    static constexpr GLint Size = 4;
    static constexpr GLenum Type = GL_UNSIGNED_INT;
    using ComponentType = unsigned int;
};

// Double precision vectors
template<>
struct GLMTypeTrait<glm::dvec2> {
    static constexpr GLint Size = 2;
    static constexpr GLenum Type = GL_DOUBLE;
    using ComponentType = double;
};

template<>
struct GLMTypeTrait<glm::dvec3> {
    static constexpr GLint Size = 3;
    static constexpr GLenum Type = GL_DOUBLE;
    using ComponentType = double;
};

template<>
struct GLMTypeTrait<glm::dvec4> {
    static constexpr GLint Size = 4;
    static constexpr GLenum Type = GL_DOUBLE;
    using ComponentType = double;
};