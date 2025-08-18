#version 330 core

layout (location = 0) in vec2 aVertexPosition;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 PassedTransform;
out vec2 TexCoords;

void main()
{
    gl_Position = PassedTransform * vec4(aVertexPosition, 0.f, 1.f);
    TexCoords = aTexCoords;
}