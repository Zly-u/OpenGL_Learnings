#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D Texture0;

void main()
{
    FragColor = texture(Texture0, TexCoord) * vec4(vertexColor, 1.f);
}