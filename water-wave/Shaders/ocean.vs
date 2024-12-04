#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform float waveLength;
uniform float steepness;
uniform vec2 waveDirection; // 波浪方向

out vec3 FragPos;
out vec3 FragNormal;

void main() {
    // 計算波浪參數
    float k = 2.0 * 3.1415926 / waveLength;  // 波數
    float c = sqrt(9.8 / k);                 // 波速
    vec2 d = normalize(waveDirection);       // 波浪方向單位向量

    // 波浪進行公式
    float f = k * (dot(d, aPos.xz) - c * time); // 波浪相位
    float a = steepness / k;                    // 波幅（振幅）

    // 修改頂點位置
    vec3 modifiedPos = aPos;
    modifiedPos.x += a * d.x * cos(f);       // 在 x 軸方向的位移
    modifiedPos.z += a * d.y * cos(f);       // 在 z 軸方向的位移
    modifiedPos.y += a * sin(f);             // 在 y 軸方向的位移（高度）

    // 計算切線和法向量
    vec3 tangent = normalize(vec3(1.0 - steepness * d.x * sin(f), steepness * cos(f), -steepness * d.y * sin(f)));
    vec3 normal = normalize(vec3(-tangent.y, tangent.x, tangent.z)); // 法向量
    FragNormal = normal;
    FragPos = modifiedPos;

    // 計算最終的頂點位置
    gl_Position = projection * view * vec4(modifiedPos, 1.0);
}
