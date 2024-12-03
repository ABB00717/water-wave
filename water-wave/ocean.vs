#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    // 波浪參數
    float amplitude = 0.01;    // 波浪高度
    float frequency = 3.0;    // 波浪頻率
    float speed = 2.0;         // 波浪移動速度

    vec3 modifiedPos = aPos;

    // 計算方向角與動態因子
    float angle = atan(aPos.z, aPos.x);  // 計算方向角
    float directionFactor = sin(angle + time); // 隨時間變化的方向因子

    // 使用梯度場影響波浪
    modifiedPos.y += amplitude * sin(frequency * (aPos.x + aPos.z) + speed * time * directionFactor);

    // 設置最終頂點位置
    gl_Position = projection * view * vec4(modifiedPos, 1.0);
}
