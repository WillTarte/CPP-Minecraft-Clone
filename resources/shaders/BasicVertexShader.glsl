#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 fragmentColor;

uniform mat4 base_mvp;
uniform mat4 transform = mat4(1.0f);

void main()
{
    gl_Position = base_mvp * transform * vec4(aPos, 1.0);
    fragmentColor = aColor;
}