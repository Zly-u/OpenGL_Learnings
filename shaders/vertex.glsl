#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 TexCoord;

uniform mat4 PassedTransform;
uniform float PassedZDepth;

void main()
{
    gl_Position = PassedTransform * vec4(aPos, PassedZDepth, 1.f);
    vertexColor = aColor;
    TexCoord = aTexCoord;
}