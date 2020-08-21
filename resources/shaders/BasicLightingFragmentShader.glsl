#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Pos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform sampler2D texture2D;
uniform samplerCube textureCubeMap;
uniform bool isCubeMap = false;

void main()
{
    vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);
    if (isCubeMap) {
        vec3 texDir = Pos - vec3(0.5, 0.5, 0.5);//Because the origin of our blocks is at corner of the model
        objectColor = texture(textureCubeMap, texDir).rgb;
    } else {
        objectColor = texture(texture2D, TexCoords).rgb;
    }

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * objectColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * objectColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * objectColor;

    vec3 result = (ambient + diffuse + specular);
    if (lightPos.y < FragPos.y) {
        result = ambient;
    }
    FragColor = vec4(result, 1.0);
}