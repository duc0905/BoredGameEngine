#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <string>

/// Compiles, links and manages a vertex + fragment shader program.
/**
 * Shader encapsulates the whole shader pipeline for rendering: it loads the
 * GLSL sources from disk, compiles the vertex and fragment stages, links them
 * into a single OpenGL program object, and exposes helpers to activate the
 * program and upload uniform values to it. The class owns the program handle
 * and releases it on destruction.
 */
class Shader {
public:
  GLuint shaderProgram; /**< The OpenGL shader program object handle */

public:
  /// Construct a Shader from a vertex and a fragment shader source file.
  /**
   * Reads the GLSL sources at @p vertexPath and @p fragmentPath, compiles the
   * vertex and fragment shaders, links them into a shader program, and deletes
   * the intermediate shader objects. File, compilation and linking errors are
   * printed to stdout but do not abort the constructor.
   *
   * @param vertexPath path to the vertex shader source file.
   *
   * @param fragmentPath path to the fragment shader source file.
   */
  Shader(const char *vertexPath, const char *fragmentPath);

  /// Destructor that releases the shader program.
  /**
   * Deletes the OpenGL program object via glDeleteProgram.
   */
  ~Shader();

  /// Activate the shader program for subsequent draw calls.
  /**
   * Binds the shader program with glUseProgram so that all following rendering
   * commands are executed with it.
   */
  void Use();

  // Utility uniform functions
  /// Set a boolean uniform in the shader.
  /**
   * Uploads the boolean as an integer (0/1) because GLSL does not expose a
   * native boolean uniform type.
   *
   * @param name the name of the uniform variable in the shader.
   *
   * @param value the boolean value to upload.
   */
  void setUniformBool(const std::string &name, bool value) const;

  /// Set an integer uniform in the shader.
  /**
   * @param name the name of the uniform variable in the shader.
   *
   * @param value the integer value to upload.
   */
  void setUniformInt(const std::string &name, int value) const;

  /// Set a float uniform in the shader.
  /**
   * @param name the name of the uniform variable in the shader.
   *
   * @param value the float value to upload.
   */
  void setUniformFloat(const std::string &name, float value) const;

  /// Set a 4x4 matrix uniform in the shader.
  /**
   * Uploads the matrix without transposition (GL_FALSE), matching glm's
   * column-major storage layout.
   *
   * @param name the name of the uniform variable in the shader.
   *
   * @param mat the 4x4 matrix to upload.
   */
  void setUniformMat4f(const std::string &name, const glm::mat4 &mat) const;

  /// Set a 3D vector uniform in the shader.
  /**
   * @param name the name of the uniform variable in the shader.
   *
   * @param vec the vec3 to upload.
   */
  void setUniformVec3f(const std::string &name, const glm::vec3 &vec) const;
}; // Shader class
