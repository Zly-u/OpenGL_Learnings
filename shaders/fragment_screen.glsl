#version 330 core

out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoords;

uniform sampler2D ScreenTexture;

void main()
{
    FragColor = texture(ScreenTexture, TexCoords) * vec4(VertexColor, 1.f);
}