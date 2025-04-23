#include "Shader.hpp"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // 1. Retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  // Ensure ifstream objects can throw exceptions
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // Open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);

    // Read file's buffer contents into streams
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // Close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // Convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure &e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
              << std::endl;
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  // 2. Compile shaders
  GLuint vertexShader, fragmentShader;
  int success;
  char infoLog[512];

  // Vertex shader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vShaderCode, NULL);
  glCompileShader(vertexShader);

  // Check for compilation errors
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Fragment shader
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
  glCompileShader(fragmentShader);

  // Check for compilation errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Shader program
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  // Delete shaders as they're linked into our program now and no longer
  // necessary
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
};

Shader::~Shader() { glDeleteProgram(shaderProgram); };

void Shader::Use() { glUseProgram(shaderProgram); };

// Utility uniform functions
void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
};

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
};

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
};

void Shader::setUniformMat4f(const std::string &name,
                             const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1,
                     GL_FALSE, glm::value_ptr(mat));
};

void Shader::setUniformVec3f(const std::string &name,
                             const glm::vec3 &vec) const {
  glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1,
               glm::value_ptr(vec));
};
