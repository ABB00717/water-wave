#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform float waveLength;
uniform float speed;
uniform float steepness;

out vec3 FragPos;
out vec3 FragNormal;

void main() {
    float k = 2 * 3.1415926 / waveLength;
    float f = k * (aPos.x - speed * time);
    float a = steepness / k;

    vec3 modifiedPos = aPos;
    modifiedPos.x += a * cos(f);
    modifiedPos.y = a * sin(f);

    vec3 tangent = normalize(vec3(1.0 - steepness * sin(f), steepness * cos(f), 0.0));
    vec3 normal = vec3(-tangent.y, tangent.x, 0.0);
    FragNormal = normal;
    FragPos = modifiedPos;
    gl_Position = projection * view * vec4(modifiedPos, 1.0);
}
