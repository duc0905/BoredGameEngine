#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <fstream>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
 public:
  GLuint shaderProgram;

 public:
  Shader(const char *vertexPath, const char *fragmentPath);

  // Destructor
  ~Shader();

  // Use/activate the shader
  void Use();

  // Utility uniform functions
  void setBool(const std::string &name, bool value) const;

  void setInt(const std::string &name, int value) const;

  void setFloat(const std::string &name, float value) const;

  void setUniformMat4f(const std::string &name, const glm::mat4 &mat) const;

  void setUniformVec3f(const std::string &name, const glm::vec3 &vec) const;
};  // Shader class
