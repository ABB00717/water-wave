#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    // 波浪參數
    float amplitude = 0.01;    // 波浪高度
    float frequency = 30.0;    // 波浪頻率
    float speed = 2.0;         // 波浪移動速度
    
    // 動態修改頂點的 y 坐標
    vec3 modifiedPos = aPos;
    // modifiedPos.y += amplitude * sin(frequency * aPos.z + speed * time);
    modifiedPos.y += amplitude * sin(frequency * (aPos.z + aPos.x) + speed * time);

    // 設置最終頂點位置
    gl_Position = projection * view * model * vec4(modifiedPos, 1.0);
    texCoord = aTexCoord;
}
