#include <Nodes/Camera.hpp>
#include <gtest/gtest.h>

// TEST(CameraTest, BasicViewMatrix) {
//   View view({0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f});
//
//   glm::mat4 viewMat = view.GetViewMatrix();
//   glm::mat4 c2w = view.GetC2WMatrix();
//
//   std::cerr << "view matrix: " << std::endl;
//   for (int i = 0; i < 4; i++) {
//     for (int j = 0; j < 4; j++) {
//       std::cerr << viewMat[j][i] << " ";
//     }
//     std::cerr << std::endl;
//   }
//
//   glm::vec4 p_original, p_transform, p_expected;
//
//   { // world origin -> Camera coord
//     p_original = {0.0f, 0.0f, 0.0f, 1.0f};
//     p_expected = {0.0f, 0.0f, -3.0f, 1.0f};
//     p_transform = viewMat * p_original;
//
//     for (int i = 0; i < 4; i++) {
//       EXPECT_FLOAT_EQ(p_transform[i], p_expected[i]);
//     }
//
//     p_transform = c2w * p_transform;
//     for (int i = 0; i < 4; i++) {
//       EXPECT_FLOAT_EQ(p_transform[i], p_original[i]);
//     }
//   }
//
//   {
//     p_original = {3.0f, 4.0f, 5.0f, 1.0f};
//     p_expected = {-3.0f, 4.0f, 2.0f, 1.0f};
//     p_transform = viewMat * p_original;
//
//     for (int i = 0; i < 4; i++) {
//       EXPECT_FLOAT_EQ(p_transform[i], p_expected[i]);
//     }
//
//     p_transform = c2w * p_transform;
//     for (int i = 0; i < 4; i++) {
//       EXPECT_FLOAT_EQ(p_transform[i], p_original[i]);
//     }
//   }
// }

TEST(CameraTest, SimpleOrthoProject) {
  Bored::Orthographic proj(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);

  glm::mat4 projMat = proj.GetProjectionMatrix();

  std::cerr << "Projection matrix: " << std::endl;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cerr << projMat[j][i] << " ";
    }
    std::cerr << std::endl;
  }

  glm::vec4 p_original, p_transform, p_expected;

  { // camera origin -> Camera coord
    p_original = {0.0f, 0.0f, 0.0f, 1.0f};
    p_expected = {-1.0f, -1.0f, 0.0f, 1.0f};
    p_transform = projMat * p_original;

    for (int i = 0; i < 4; i++) {
      EXPECT_FLOAT_EQ(p_transform[i], p_expected[i]);
    }
  }

  { // camera origin -> Camera coord
    p_original = {1.0f, 1.0f, 1.0f, 1.0f};
    p_expected = {1.0f, 1.0f, 1.0f, 1.0f};
    p_transform = projMat * p_original;

    for (int i = 0; i < 4; i++) {
      EXPECT_FLOAT_EQ(p_transform[i], p_expected[i]);
    }
  }
}
