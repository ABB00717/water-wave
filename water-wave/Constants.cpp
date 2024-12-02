#include "Constants.h"

// Window size
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

// Grid settings
std::vector<float> vertices = {};
std::vector<unsigned int> indices = {};
int gridSize = 100;
float spacing = 0.01f;

// Camera settings
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = SCREEN_WIDTH/2;
float lastY = SCREEN_HEIGHT/2;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));