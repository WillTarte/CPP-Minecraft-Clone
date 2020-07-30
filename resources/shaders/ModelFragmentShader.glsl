#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    FragColor = /*vec4(1.0f, 1.0f, 1.0f, 1.0f);*/vec4(texture(texture1, TexCoord).rgb, 1.0f);
}