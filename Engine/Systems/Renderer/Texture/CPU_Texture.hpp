#pragma once

#include "../I_Texture.hpp"
#include <cstddef>
#include <stdexcept>

/**
 * In memory 2D texture.
 */
class CPU_Texture2D : public I_Texture2D {
public:
  CPU_Texture2D() = default;
  CPU_Texture2D(CPU_Texture2D &&) = default;
  CPU_Texture2D(const CPU_Texture2D &) = default;
  CPU_Texture2D &operator=(CPU_Texture2D &&) = default;
  CPU_Texture2D &operator=(const CPU_Texture2D &) = default;
  ~CPU_Texture2D() = default;

  /**
   * Copy from another I_Texture.
   *
   * @param other The texture to copy from.
   */
  CPU_Texture2D(I_Texture2D &other) {
    WriteData(other.GetData(), other.GetSize(), other.GetBPP());
  }

public:
  [[nodiscard]] inline virtual int GetBPP() const { return m_bpp; }

  [[nodiscard]] inline virtual glm::vec<2, int> GetSize() const {
    return m_size;
  }

  [[nodiscard]] inline virtual std::vector<std::byte> GetData() const {
    return m_data;
  }

  inline virtual void WriteData(const std::vector<std::byte> &data,
                                const glm::vec2 &size, int bpp) {
    size_t real_size = size[0] * size[1] * bpp;
    if (data.size() != real_size) {
      throw std::runtime_error(
          "[Error]: Size of data is not equals to width * height * bpp");
    }

    m_bpp = bpp;
    m_size = size;
    m_data = data;
  }

private:
  int m_bpp;
  glm::vec2 m_size;
  std::vector<std::byte> m_data;
};
