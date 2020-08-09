#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 Pos;

uniform samplerCube texture1;

void main()
{
    vec3 texDir = Pos - vec3(0.5, 0.5, 0.5);//Because the origin of our blocks is a corner of the model
    FragColor = vec4(texture(texture1, texDir).rgb, 1.0f);
}