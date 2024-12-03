#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    // 波浪參數
    float amplitude = 0.1;    // 波浪高度
    float frequency = 3.0;    // 波浪頻率
    float speed = 2.0;        // 波浪移動速度

    vec3 modifiedPos = aPos;

    // 使用梯度場影響波浪
    modifiedPos.y += amplitude * sin(frequency * (aPos.x + aPos.z) + speed * time);

    // 設置最終頂點位置
    gl_Position = projection * view * vec4(modifiedPos, 1.0);
}
