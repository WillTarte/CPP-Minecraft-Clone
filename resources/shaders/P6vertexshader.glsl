#version 330

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;

// Values that stay constant for the whole mesh.
uniform mat4 base_mvp;
uniform mat4 transform = mat4(1.0);

void main() {
    gl_Position = base_mvp * transform * vec4(aPos, 1.0f);
}
