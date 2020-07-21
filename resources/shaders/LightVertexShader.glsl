#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;

// Values that stay constant for the whole mesh.
uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 projection = mat4(1.0f);
uniform mat4 local_transform = mat4(1.0f);

void main(){
    // Output position of the vertex, in clip space : MVP * position
    gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
}