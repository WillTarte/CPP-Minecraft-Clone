#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

// Values that stay constant for the whole mesh.
uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 projection = mat4(1.0f);
uniform mat4 local_transform = mat4(1.0f);

void main() {
    // Output position of the vertex, in clip space : MVP * position
    FragPos = vec3(model * local_transform * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
    Normal = mat3(transpose(inverse(model*local_transform))) * aNormal;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
