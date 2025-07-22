#pragma once
#include <cstddef>
#include <glm/glm.hpp>
#include <vector>

/**
 * Interface to define a 2D texture.
 */
class I_Texture2D {
public:
  virtual ~I_Texture2D() = default;

  /**
   * Get the BPP - Bytes per pixel of the image.
   */
  [[nodiscard]] virtual int GetBPP() const = 0;

  /**
   * Get the 2D size of the image.
   *
   * @return glm::vec2 The width and height of the image;
   */
  [[nodiscard]] virtual glm::vec<2, int> GetSize() const = 0;

  /**
   * Get the pixel data of the image.
   *
   * The size of the vector should be width * height * bpp.
   *
   * @return The vector containing every byte of the pixel data.
   */
  [[nodiscard]] virtual std::vector<std::byte> GetData() const = 0;

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
   */
  virtual void WriteData(const std::vector<std::byte> &data,
                         const glm::vec2 &size, int bpp) = 0;
};
