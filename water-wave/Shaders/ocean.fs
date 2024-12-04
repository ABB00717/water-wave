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

    // 折射
    float ratio = 1.00 / 1.33; // 空氣到水的折射率
    vec3 I = normalize(FragPos - viewPos);
    vec3 refractDir = refract(I, norm, ratio); // 計算折射方向
    vec4 refractedColor = vec4(texture(skybox, refractDir).rgb, 1.0);

    // 結合反射與折射
    vec3 R = reflect(I, norm);
    vec4 reflectedColor = vec4(texture(skybox, R).rgb, 1.0);

    // 混合折射、反射與光照
    vec4 baseColor = ambient + diffuse + specular;
    FragColor = mix(refractedColor, reflectedColor, 0.7) * oceanColor * 0.7 + baseColor * 0.3;
}
