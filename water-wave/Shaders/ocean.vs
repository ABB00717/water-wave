#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec3 FragPos;
out vec3 FragNormal;

void main() {
    vec3 normal = vec3(0.0f, 1.0f, 0.0f);

    FragNormal = normal;
    FragPos = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}
