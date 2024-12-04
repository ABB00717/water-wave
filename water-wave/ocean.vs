#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec3 FragPos;
out vec3 FragNormal;

void main() {
    float amplitude = 0.8;
    float frequency = 0.5;
    float speed = 5.0;

    vec3 modifiedPos = aPos;
    float waveArg = (frequency * (aPos.x + aPos.z) + speed * time);
    float waveArgHehe = (frequency * (aPos.x) + speed * time);
    modifiedPos.x += amplitude * sin(waveArgHehe);

    // modifiedPos.y += amplitude * sin(waveArg);
    // float dYdx = amplitude * frequency * cos(waveArg);
    // float dYdz = amplitude * frequency * cos(waveArg);

    modifiedPos.y += amplitude * sin(waveArgHehe);
    float dYdx = amplitude * frequency * cos(waveArgHehe);
    float dYdz = amplitude * frequency * cos(waveArgHehe);

    vec3 normal = normalize(vec3(-dYdx, 1.0, -dYdz));

    FragNormal = normal;
    FragPos = modifiedPos;
    gl_Position = projection * view * vec4(modifiedPos, 1.0);
}
