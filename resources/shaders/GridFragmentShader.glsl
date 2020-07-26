#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D textureSampler;

void main()
{
    vec3 objectColor = texture(textureSampler, TexCoord).rrr;

    FragColor = vec4(objectColor, 1.0f);
}