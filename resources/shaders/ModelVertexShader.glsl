#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
// for textures layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
//vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

// Values that stay constant for the whole mesh.
uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 projection = mat4(1.0f);
uniform mat4 local_transform = mat4(1.0f);
uniform mat4 lightSpaceMatrix = mat4(1.0f);

void main() {
    // Output position of the vertex, in clip space : MVP * position
    vs_out.FragPos = vec3(model * local_transform * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model*local_transform))) * aNormal;
    //vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);

}
