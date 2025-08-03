// clang-format off
#include <Systems/Renderer/Texture/CPU_Texture.hpp>
#include <Systems/Renderer/Texture/OGL_Texture.hpp>
#include <GLFW/glfw3.h>
// clang-format on
#include <gtest/gtest.h>
#include <stdexcept>

///////////// CPU Texture //////////////

/**
 * 1x1x3 texture, provide 3 bytes.
 */
TEST(CPUTextureTest, SimpleValid) {
  glm::vec<2, int> size{1, 1};
  int bpp = 3;
  std::vector<std::byte> data = {std::byte{10}, std::byte{20}, std::byte{30}};

  CPU_Texture2D texture;

  EXPECT_NO_THROW({ texture.WriteData(data, size, bpp); });

  EXPECT_EQ(texture.GetSize(), size);
  EXPECT_EQ(texture.GetBPP(), bpp);
  EXPECT_EQ(texture.GetData(), data);
}

/**
 * 1x1x3 texture, provide 3 bytes.
 */
TEST(CPUTextureTest, SimpleCopy) {
  glm::vec<2, int> size{1, 1};
  int bpp = 3;
  std::vector<std::byte> data = {std::byte{10}, std::byte{20}, std::byte{30}};

  CPU_Texture2D originalTexture;

  EXPECT_NO_THROW({ originalTexture.WriteData(data, size, bpp); });

  // Call copy-constructor
  CPU_Texture2D copyTexture(originalTexture);

  // Expect similar content
  EXPECT_EQ(originalTexture.GetBPP(), copyTexture.GetBPP());
  EXPECT_EQ(originalTexture.GetSize(), copyTexture.GetSize());
  EXPECT_EQ(originalTexture.GetData(), copyTexture.GetData());

  std::vector<std::byte> newData = {std::byte{80}, std::byte{70}, std::byte{60},
                                    std::byte{50}, std::byte{40}, std::byte{30},
                                    std::byte{20}, std::byte{10}};
  glm::vec<2, int> newSize{2, 2};
  EXPECT_NO_THROW({ copyTexture.WriteData(newData, newSize, 2); });

  // Expect write to work
  EXPECT_EQ(copyTexture.GetBPP(), 2);
  EXPECT_EQ(copyTexture.GetSize(), newSize);
  EXPECT_EQ(copyTexture.GetData(), newData);

  // Expect did not override original texture
  EXPECT_EQ(originalTexture.GetBPP(), bpp);
  EXPECT_EQ(originalTexture.GetSize(), size);
  EXPECT_EQ(originalTexture.GetData(), data);
}

/**
 * 1x1x4 texture, but only provide 3 values.
 */
TEST(CPUTextureTest, SimpleInvalid) {
  glm::vec<2, int> size{1, 1};
  int bpp = 4;
  std::vector<std::byte> data = {std::byte{10}, std::byte{20}, std::byte{30}};

  CPU_Texture2D texture;

  ASSERT_THROW({ texture.WriteData(data, size, bpp); }, std::runtime_error);
}

/**
 * TODO
 *
 * @todo TODO: Add more complex examples
 */

///////////// OGL Texture //////////////

class OGLTextureTest : public testing::Test {
protected:
  static void SetUpTestSuite() {
    glfwInit();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    m_window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    ASSERT_NE(m_window, nullptr);
    glfwMakeContextCurrent(m_window);
    ASSERT_TRUE(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
  }

  static void TearDownTestSuite() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }

  static GLFWwindow *m_window;
};
GLFWwindow *OGLTextureTest::m_window = nullptr;

/**
 * 1x1x3 texture, provide 3 bytes.
 */
TEST_F(OGLTextureTest, SimpleValid) {
  glm::vec<2, int> size{1, 1};
  int bpp = 3;
  std::vector<std::byte> data = {std::byte{10}, std::byte{20}, std::byte{30}};

  OGL_Texture2D texture;

  EXPECT_NO_THROW({ texture.WriteData(data, size, bpp); });

  EXPECT_EQ(texture.GetSize(), size);
  EXPECT_EQ(texture.GetBPP(), bpp);
  EXPECT_EQ(texture.GetData(), data);
}

/**
 * 1x1x3 texture, provide 3 bytes.
 */
TEST_F(OGLTextureTest, SimpleCopy) {
  glm::vec<2, int> size{1, 1};
  int bpp = 3;
  std::vector<std::byte> data = {std::byte{10}, std::byte{20}, std::byte{30}};

  OGL_Texture2D originalTexture;

  EXPECT_NO_THROW({ originalTexture.WriteData(data, size, bpp); });

  // Call copy-constructor
  OGL_Texture2D copyTexture(originalTexture);

  // Expect similar content
  EXPECT_EQ(originalTexture.GetBPP(), copyTexture.GetBPP());
  EXPECT_EQ(originalTexture.GetSize(), copyTexture.GetSize());
  EXPECT_EQ(originalTexture.GetData(), copyTexture.GetData());

  std::vector<std::byte> newData = {std::byte{80}, std::byte{70}, std::byte{60},
                                    std::byte{50}, std::byte{40}, std::byte{30},
                                    std::byte{20}, std::byte{10}};
  glm::vec<2, int> newSize{2, 2};
  EXPECT_NO_THROW({ copyTexture.WriteData(newData, newSize, 2); });

  // Expect write to work
  EXPECT_EQ(copyTexture.GetBPP(), 2);
  EXPECT_EQ(copyTexture.GetSize(), newSize);
  EXPECT_EQ(copyTexture.GetData(), newData);

  // Expect did not override original texture
  EXPECT_EQ(originalTexture.GetBPP(), bpp);
  EXPECT_EQ(originalTexture.GetSize(), size);
  EXPECT_EQ(originalTexture.GetData(), data);
}

/**
 * 1x1x4 texture, but only provide 3 values.
 */
TEST_F(OGLTextureTest, SimpleInvalid) {
  glm::vec<2, int> size{1, 1};
  int bpp = 4;
  std::vector<std::byte> data = {std::byte{10}, std::byte{20}, std::byte{30}};

  OGL_Texture2D texture;

  ASSERT_THROW({ texture.WriteData(data, size, bpp); }, std::runtime_error);
}
