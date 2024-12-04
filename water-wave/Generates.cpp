#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Generates.h"

void generateTexture(unsigned int* texture, const char* imgPath) {
  // 讀取圖片
  // xstbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
  unsigned char* data = stbi_load(imgPath, &width, &height, &nrChannels, 0);
  // 生成紋理以及Mipmap
  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // 載入紋理
  if (data) {
    if (nrChannels==4)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else if (nrChannels==3)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout<<"Failed to load texture"<<std::endl;
  }
  stbi_image_free(data);
}

void generateGrid(std::vector<float>& vertices, std::vector<unsigned int>& indices, int gridSize, float spacing) {
  for (int z = 0; z<=gridSize; ++z) {
    for (int x = 0; x<=gridSize; ++x) {
      vertices.push_back(x*spacing);                // x
      vertices.push_back(0.0f);                     // y
      vertices.push_back(z*spacing);                // z
    }
  }
  for (int z = 0; z<gridSize; ++z) {
    for (int x = 0; x<gridSize; ++x) {
      int topLeft = z*(gridSize+1)+x;
      int topRight = topLeft+1;
      int bottomLeft = (z+1)*(gridSize+1)+x;
      int bottomRight = bottomLeft+1;
      indices.push_back(topLeft);
      indices.push_back(bottomLeft);
      indices.push_back(topRight);
      indices.push_back(topRight);
      indices.push_back(bottomLeft);
      indices.push_back(bottomRight);
    }
  }
}

void generateSkybox(unsigned int* texture, std::vector<std::string> faces) {
  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
  int width, height, nrChannels;
  for (unsigned int i = 0; i<faces.size(); ++i) {
    unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout<<"Failed to load texture"<<std::endl;
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void generateBuffers(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
void generateBuffers(unsigned int& VAO, unsigned int& VBO, const std::vector<float>& vertices) {
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
