#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 FragNormal;

uniform samplerCube skybox;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec4 oceanColor;
uniform vec4 lightColor;

void main() {
    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // 漫反射
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    // 環境光
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * lightColor;

    // 鏡面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = 0.4 * pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    vec4 specular = spec * lightColor;

    vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, normalize(FragNormal));
    FragColor = mix(vec4(texture(skybox, R).rgb, 1.0), (ambient + diffuse + specular) * oceanColor, 0.3);
}
