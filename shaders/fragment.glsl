#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D Texture0;

void main()
{
    vec4 TexColor = texture(Texture0, TexCoord);

    if(TexColor.a < 1.f/255.f)
    {
        discard;
    }

    FragColor = TexColor * vec4(vertexColor, 1.f);
}