#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D Texture0;

uniform float PixelationLevel;

void main()
{
    vec2 TexSize = vec2(textureSize(Texture0, 0));
    vec2 UV = TexCoord;

    vec2 blockUV = floor(UV * TexSize / PixelationLevel) * PixelationLevel / TexSize;

    FragColor = texture(Texture0, blockUV) * vec4(vertexColor, 1.f);
}