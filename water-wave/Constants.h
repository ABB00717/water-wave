#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Camera.h"

// Window size
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// Grid settings
extern std::vector<float> vertices;
extern std::vector<unsigned int> indices;
const extern std::vector<float> lightCubeVertices;
const extern std::vector<float> skyboxVertices;
const extern std::vector<std::string> skyboxFaces;
extern int gridSize;
extern float spacing;

// Camera settings
extern bool firstMouse;
extern float deltaTime;
extern float lastFrame;
extern float lastX;
extern float lastY;
extern Camera camera;