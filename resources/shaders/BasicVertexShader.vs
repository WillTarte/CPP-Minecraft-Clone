#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 fragmentColor;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


void main()
{
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	fragmentColor = aColor;
}