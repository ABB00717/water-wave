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

  Shader lightShader("./light.vs", "./light.fs");
  Shader oceanShader("./ocean.vs", "./ocean.fs");

  generateGrid(vertices, indices, gridSize, spacing);

  // 綁定 VBO, VAO, EBO
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STREAM_DRAW);
  // 告訴OpenGL如何解析頂點數據
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  unsigned int lightVAO, lightVBO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glGenBuffers(1, &lightVBO);
  glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  glBufferData(GL_ARRAY_BUFFER, lightCubeVertices.size()*sizeof(float), lightCubeVertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 設定清除顏色
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // 清除顏色緩衝

    // 啟動著色器並綁定紋理
    oceanShader.use();
    oceanShader.setFloat("time", currentFrame);
    oceanShader.setVec4("lightColor", glm::vec4(1.0f));
    oceanShader.setVec4("oceanColor", glm::vec4(0.30f, 0.51f, 0.66f, 1.0f));
    oceanShader.setVec3("lightPos", glm::vec3(0.0f, 5.0f, 0.0f));
    oceanShader.setVec3("viewPos", camera.Position);

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);

    unsigned int viewLoc = glGetUniformLocation(oceanShader.ID, "view");
    unsigned int projectionLoc = glGetUniformLocation(oceanShader.ID, "projection");
    oceanShader.setMat4("view", view);
    oceanShader.setMat4("projection", projection);

    // 繪製海洋
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // 繪製光源
    lightShader.use();
    glm::mat4 lightModel = glm::mat4(1.0f); // 初始化單位矩陣
    lightModel = glm::translate(lightModel, glm::vec3(0.0f, 5.0f, 0.0f)); // 平移到 (5.0f, 10.0f, 5.0f)
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