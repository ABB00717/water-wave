#ifndef GENERATES_H
#define GENERATES_H

#include <iostream>
#include <filesystem>
#include <direct.h>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

void generateTexture(unsigned int* texture, const char* imgPath);
void generateGrid(std::vector<float>& vertices, std::vector<unsigned int>& indices, int gridSize, float spacing);

#endif