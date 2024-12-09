#include <iostream>
#include <filesystem>
#include <direct.h>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Shader.h"
#include "Camera.h"
#include "Constants.h"
#include "Callbacks.h"
#include "Generates.h"

int main() {
  // Init GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a window
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Learn OpenGL", nullptr, nullptr);
  if (window==nullptr) return -1;
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  // Init Glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;

  Shader lightShader("./Shaders/light.vs", "./Shaders/light.fs");
  Shader oceanShader("./Shaders/ocean.vs", "./Shaders/ocean.fs");
  Shader skyboxShader("./Shaders/skybox.vs", "./Shaders/skybox.fs");

  unsigned int skyboxTexture;
  glGenTextures(1, &skyboxTexture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
  generateSkybox(&skyboxTexture, skyboxFaces);
  generateGrid(vertices, indices, gridSize, spacing);

  unsigned int oceanVAO, oceanVBO, oceanEBO;
  generateBuffers(oceanVAO, oceanVBO, oceanEBO, vertices, indices);
  unsigned int lightVAO, lightVBO;
  generateBuffers(lightVAO, lightVBO, lightCubeVertices);
  unsigned int skyboxVAO, skyboxVBO;
  generateBuffers(skyboxVAO, skyboxVBO, skyboxVertices);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  float waveLength = 3.0f, steepness = 0.2;
  glm::vec2 waveDirection = glm::vec2(1.0, 1.0);
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // 設定視口大小
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback); // 設定視窗大小改變時的callback
  while (!glfwWindowShouldClose(window)) {
    float cameraSpeed = 7.5f*deltaTime;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame-lastFrame;
    lastFrame = currentFrame;

    processInput(window); // 檢查是否按下ESC
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 渲染指令
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 線框模式
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 設定清除顏色
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // 清除顏色緩衝

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // 繪製天空盒
    glDepthMask(GL_FALSE);
    skyboxShader.use();
    glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix())); // 移除平移部分
    skyboxShader.setMat4("view", skyboxView);
    skyboxShader.setMat4("projection", projection);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);

    glm::vec2 wave[2] = {
      glm::vec2(1.0f, 0.1f),
      glm::vec2(1.0f, 0.1f)
    };
    glm::vec2 waveDir[2] = {
      glm::vec2(1.0f, 0.0f),
      glm::vec2(0.3f, 0.3f)
    };
    // 啟動著色器並綁定紋理
    oceanShader.use();
    oceanShader.setFloat("time", currentFrame);
    oceanShader.setVec4("lightColor", glm::vec4(1.0f));
    oceanShader.setVec4("oceanColor", glm::vec4(0.3f, 0.51f, 0.66f, 1.0f));
    oceanShader.setVec3("lightPos", glm::vec3(5.0f, 2.0f, 5.0f));
    oceanShader.setVec2Array("wave", 2, wave);
    oceanShader.setVec2Array("waveDir", 2, waveDir);
    //oceanShader.setFloat("waveLength", waveLength);
    //oceanShader.setFloat("steepness", steepness);
    //oceanShader.setVec2("waveDirection", waveDirection);
    oceanShader.setVec3("viewPos", camera.Position);
    unsigned int viewLoc = glGetUniformLocation(oceanShader.ID, "view");
    unsigned int projectionLoc = glGetUniformLocation(oceanShader.ID, "projection");
    oceanShader.setMat4("view", view);
    oceanShader.setMat4("projection", projection);

    // 繪製海洋
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(oceanVAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // 繪製光源
    lightShader.use();
    glm::mat4 lightModel = glm::mat4(1.0f); // 初始化單位矩陣
    lightModel = glm::translate(lightModel, glm::vec3(5.0f, 2.0f, 5.0f)); // 平移到 (5.0f, 10.0f, 5.0f)
    lightModel = glm::scale(lightModel, glm::vec3(0.2f)); // 可選：縮小光源以區分光源立方體
    lightShader.setMat4("model", lightModel);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ImGui
    //ImGui::Begin("Settings");
    //ImGui::Text("Hello, world!");
    //ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window); // 雙緩衝區交換
    glfwPollEvents(); // 檢查有沒有觸發事件
  }

  glfwTerminate();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  return 0;
}