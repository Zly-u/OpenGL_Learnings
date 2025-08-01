#version 330 core

layout (location = 0) in vec2 aVertexPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 TexCoord;

uniform mat4 PassedTransform;
uniform mat4 PassedProjection;

void main()
{
    gl_Position = PassedProjection * PassedTransform * vec4(aVertexPosition, 0.f, 1.f);
    vertexColor = aColor;
    TexCoord = aTexCoord;
}