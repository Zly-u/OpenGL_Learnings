#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D Texture;

void main()
{
//    FragColor = vec4(vertexColor, 1.f);
    FragColor = texture(Texture, TexCoord);
}