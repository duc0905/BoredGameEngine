#pragma once
#include "../I_Texture.hpp"
#include "CPU_Texture.hpp"
#include <cstddef>
#include <format>
#include <glad/glad.h>
#include <iostream>
#include <ostream>
#include <stdexcept>

/**
 * Wrapper for OpenGL 2D Texture.
 */
class OGL_Texture2D : public I_Texture2D {
public:
  /**
   * Create an OpenGL 2D Texture.
   */
  OGL_Texture2D() {
    glGenTextures(1, &m_texId);
    glBindTexture(GL_TEXTURE_2D, m_texId);

    // Set the params for the texture.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  /**
   * Move constructor.
   *
   * Only copy the texture ID.
   */
  OGL_Texture2D(OGL_Texture2D &&other) { m_texId = other.m_texId; }

  /**
   * Copy constructor.
   *
   * Copy the content of other texture to this texture.
   */
  OGL_Texture2D(const OGL_Texture2D &other) : OGL_Texture2D() {
    WriteData(other.GetData(), other.GetSize(), other.GetBPP());
  }

  /**
   * Move constructor.
   *
   * Only copy the texture ID.
   */
  OGL_Texture2D &operator=(OGL_Texture2D &&other) {
    m_texId = other.m_texId;
    return *this;
  }

  /**
   * Copy constructor.
   *
   * Copy the content of other texture to this texture.
   */
  OGL_Texture2D &operator=(const OGL_Texture2D &other) {
    WriteData(other.GetData(), other.GetSize(), other.GetBPP());
    return *this;
  }

  /**
   * Copy constructor.
   *
   * Copy the content of another CPU_Texture2D.
   */
  OGL_Texture2D(const I_Texture2D &other) : OGL_Texture2D() {
    WriteData(other.GetData(), other.GetSize(), other.GetBPP());
  }

  ~OGL_Texture2D() { glDeleteTextures(1, &m_texId); }

public:
  /**
   * Get the BPP - Bytes per pixel of the image.
   */
  [[nodiscard]] virtual int GetBPP() const {
    int format;
    glGetTextureLevelParameteriv(m_texId, 0, GL_TEXTURE_INTERNAL_FORMAT,
                                 &format);

    switch (format) {
    case GL_RED:
      return 1;
    case GL_RG:
      return 2;
    case GL_RGB:
      return 3;
    case GL_RGBA:
      return 4;
    default:
      throw std::runtime_error(
          std::format("Unknown OpenGL internal texture format: {}", format));
    }
  }

  /**
   * Get the 2D size of the image.
   *
   * @return glm::vec2 The width and height of the image;
   */
  [[nodiscard]] virtual glm::vec<2, int> GetSize() const {
    int width, height;
    glGetTextureLevelParameteriv(m_texId, 0, GL_TEXTURE_WIDTH, &width);
    glGetTextureLevelParameteriv(m_texId, 0, GL_TEXTURE_HEIGHT, &height);

    return {width, height};
  }

  /**
   * Get the pixel data of the image.
   *
   * The size of the vector should be width * height * bpp.
   *
   * @return The vector containing every byte of the pixel data.
   */
  [[nodiscard]] virtual std::vector<std::byte> GetData() const {
    int bpp = GetBPP();
    glm::vec<2, int> size = GetSize();

    std::size_t buf_size = size[0] * size[1] * bpp;
    std::vector<std::byte> ret(buf_size, std::byte{41});

    GLenum format;
    switch (bpp) {
    case 1:
      format = GL_RED;
      break;
    case 2:
      format = GL_RG;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      throw std::runtime_error("bpp can only be 1, 2, 3, or 4.");
    }

    glBindTexture(GL_TEXTURE_2D, m_texId);
    glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, ret.data());

    return ret;
  }

  /**
   * Write the data to the texture.
   *
   * This will override the data of the texture.
   *
   * The size of the vector should be width * height * bpp.
   *
   * @param data The byte data to write.
   *
   * @param size The width and height of the new texture.
   *
   * @param bpp The bytes per pixel of the new texture.
   *
   */
  virtual void WriteData(const std::vector<std::byte> &data,
                         const glm::vec2 &size, int bpp) {
    std::size_t real_size = size[0] * size[1] * bpp;
    if (data.size() != real_size) {
      throw std::runtime_error(
          "[Error]: Size of data is not equals to width * height * bpp");
    }

    GLenum format, internalFormat;
    switch (bpp) {
    case 1:
      format = GL_RED;
      internalFormat = GL_R8;
      break;
    case 2:
      format = GL_RG;
      internalFormat = GL_RG8;
      break;
    case 3:
      format = GL_RGB;
      internalFormat = GL_RGB8;
      break;
    case 4:
      format = GL_RGBA;
      internalFormat = GL_RGBA8;
      break;
    default:
      throw std::runtime_error("bpp can only be 1, 2, 3, or 4.");
    }

    glBindTexture(GL_TEXTURE_2D, m_texId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, size[0], size[1], 0, format,
                 GL_UNSIGNED_BYTE, data.data());
    glBindTexture(GL_TEXTURE_2D, 0);
  }

public:
  GLuint m_texId;
};
