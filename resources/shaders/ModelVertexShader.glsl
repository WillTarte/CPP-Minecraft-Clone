#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;

// Values that stay constant for the whole mesh.
uniform mat4 base_mvp = mat4(1.0f);
uniform mat4 transform = mat4(1.0f);

void main(){
    // Output position of the vertex, in clip space : MVP * position
    gl_Position = base_mvp * transform*vec4(aPos, 1.0);
}