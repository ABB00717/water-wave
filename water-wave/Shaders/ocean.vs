#version 330 core
layout (location = 0) in vec3 aPos;

#define NUM_WAVE 2

uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform vec2 wave[NUM_WAVE]; // (waveLength, steepness)
uniform vec2 waveDir[NUM_WAVE];

out vec3 FragPos;
out vec3 FragNormal;

void main() {
    vec3 modifiedPos = aPos;
    vec3 totalNormal = vec3(0.0);

    for (int i = 0; i < NUM_WAVE; i++) {
      // 計算波浪參數
      float k = 2.0 * 3.1415926 / wave[i][0];  // 波數
      float c = sqrt(9.8 / k);                 // 波速
      vec2 d = normalize(waveDir[i]);          // 波浪方向單位向量

      // 波浪進行公式
      float f = k * (dot(d, aPos.xz) - c * time); // 波浪相位
      float a = wave[i][1] / k;                   // 波幅（振幅）

      // 修改頂點位置
      modifiedPos.x += a * d.x * cos(f);       // 在 x 軸方向的位移
      modifiedPos.z += a * d.y * cos(f);       // 在 z 軸方向的位移
      modifiedPos.y += a * sin(f);             // 在 y 軸方向的位移（高度）

      // 計算切線和法向量
      // 計算切線向量
      vec3 tangent = normalize(vec3(
          1.0 - wave[i][1] * d.x * d.x * sin(f),
          d.x * wave[i][1] * cos(f),
          -d.x * wave[i][1] * d.y * sin(f)
      ));
      
      // 計算雙切線 (Binormal)
      vec3 binormal = normalize(vec3(
          -d.x * d.y * (wave[i][1] * sin(f)),
          d.y * (wave[i][1] * cos(f)),
          1.0 - d.y * d.y * (wave[i][1] * sin(f))
      ));
      
      // 計算法向量
      totalNormal += cross(tangent, binormal);
    }
    
    FragNormal = normalize(totalNormal);
    FragPos = modifiedPos;

    // 計算最終的頂點位置
    gl_Position = projection * view * vec4(modifiedPos, 1.0);
}
