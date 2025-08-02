#version 330 core

layout (location = 0) in vec2 aVertexPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec3 VertexColor;
out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aVertexPos, 0.0, 1.0);
    VertexColor = aColor;
    TexCoords = aTexCoords;
}