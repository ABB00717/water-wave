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

  Shader ourShader("./shader.vs", "./shader.fs");

  // unsigned int texture1;
  // generateTexture(&texture1, "./container.jpg");

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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


  // 告訴OpenGL如何解析頂點數據
  // 位置屬性
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
  // 紋理坐標屬性
  // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
  // glEnableVertexAttribArray(1);

  // 解綁 VBO, VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

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
    ourShader.use();
    ourShader.setFloat("time", currentFrame);
    // ourShader.setInt("texture1", 0);

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);

    unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);

    // 繪製物件
    glEnable(GL_DEPTH_TEST);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture1);
    glBindVertexArray(VAO);
    glm::mat4 model = glm::mat4(1.0f);
    ourShader.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // ImGui
    ImGui::Begin("Settings");
    ImGui::Text("Hello, world!");
    ImGui::End();

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