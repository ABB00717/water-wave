#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec3 FragPos;
out vec3 FragNormal;

void main() {
    float amplitude = 0.1;
    float frequency = 3.0;
    float speed = 2.0;

    vec3 modifiedPos = aPos;
    float waveArg1 = (frequency * (aPos.x + aPos.z) + speed * time);
    float waveArg2 = (frequency * (aPos.x) + speed * time);

    // modifiedPos.y += amplitude * sin(waveArg);
    // float dYdx = amplitude * frequency * cos(waveArg1);
    // float dYdz = amplitude * frequency * cos(waveArg1);

    // modifiedPos.y += amplitude * sin(waveArg2);
    // float dYdx = amplitude * frequency * cos(waveArg2);
    // float dYdz = amplitude * frequency * cos(waveArg2);

    float wave1 = amplitude * sin(frequency * (aPos.x + aPos.z) + speed * time);
    float wave2 = amplitude * sin(frequency * (aPos.x - aPos.z) + speed * time);
    modifiedPos.y += wave1 + wave2;

    float dYdx1 = amplitude * frequency * cos(waveArg1);
    float dYdx2 = amplitude * frequency * cos(waveArg2);
    float dYdz1 = amplitude * frequency * cos(waveArg1);
    float dYdz2 = amplitude * frequency * cos(waveArg2);
    
    vec3 normal = normalize(vec3(-(dYdx1 + dYdx2), 1.0, -(dYdz1 + dYdz2)));

    FragNormal = normal;
    FragPos = modifiedPos;
    gl_Position = projection * view * vec4(modifiedPos, 1.0);
}
