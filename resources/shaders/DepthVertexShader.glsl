#version 330

layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform mat4 local_transform = mat4(1.0f);

void main()
{
    gl_Position = lightSpaceMatrix * model * local_transform * vec4(aPos, 1.0);
}