#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 Pos;

uniform sampler2D texture2D;
uniform samplerCube textureCubeMap;
uniform bool isCubeMap = false;

void main()
{
    if (isCubeMap) {
        vec3 texDir = Pos - vec3(0.5, 0.5, 0.5);//Because the origin of our blocks is at corner of the model
        FragColor = vec4(texture(textureCubeMap, texDir).rgb, 1.0f);
    } else {
        FragColor = vec4(texture(texture2D, TexCoord).rgb, 1.0f);
    }
}