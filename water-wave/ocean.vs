#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec3 FragPos;
out vec3 FragNormal;

void main() {
    vec3 modifiedPos = aPos;
    modifiedPos.y = sin(modifiedPos.x);

    vec3 normal = normalize(vec3(0.0, 1.0, 0.0));
    FragNormal = normal;
    FragPos = modifiedPos;
    gl_Position = projection * view * vec4(modifiedPos, 1.0);
}
