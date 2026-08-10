#include <Components/TransformComponent.hpp>
#include <glm/glm.hpp>
#include <gtest/gtest.h>

namespace {

/**
 * Apply an affine transform matrix to a 3D point.
 */
glm::vec3 Apply(const glm::mat4 &matrix, const glm::vec3 &point) {
  glm::vec4 result = matrix * glm::vec4(point, 1.0f);
  return glm::vec3(result);
}

/**
 * Compare two vectors with a small tolerance to absorb trig precision loss.
 */
void ExpectVec3Eq(const glm::vec3 &actual, const glm::vec3 &expected) {
  EXPECT_NEAR(actual.x, expected.x, 1e-5f);
  EXPECT_NEAR(actual.y, expected.y, 1e-5f);
  EXPECT_NEAR(actual.z, expected.z, 1e-5f);
}

/**
 * Compare two matrices element-wise with a small tolerance.
 */
void ExpectMatrixEq(const glm::mat4 &actual, const glm::mat4 &expected) {
  for (int col = 0; col < 4; ++col) {
    for (int row = 0; row < 4; ++row) {
      EXPECT_NEAR(actual[col][row], expected[col][row], 1e-5f);
    }
  }
}

} // namespace

///////////// Default construction //////////////

/**
 * A default-constructed transform must be the identity matrix.
 */
TEST(TransformComponentTest, DefaultConstruction_IsIdentity) {
  Bored::TransformComponent transform;

  ExpectMatrixEq(transform.GetTransformMatrix(), glm::mat4(1.0f));
}

/**
 * A default-constructed transform has zero translation, zero rotation and
 * unit scale.
 */
TEST(TransformComponentTest, DefaultConstruction_MemberDefaults) {
  Bored::TransformComponent transform;

  EXPECT_EQ(transform.translate, glm::vec3(0.0f));
  EXPECT_EQ(transform.rotate, glm::vec3(0.0f));
  EXPECT_EQ(transform.scale, glm::vec3(1.0f));
}

///////////// Translation //////////////

/**
 * Translating by (1, 2, 3) moves every point by that offset.
 */
TEST(TransformComponentTest, Translation_MovesOrigin) {
  Bored::TransformComponent transform;
  transform.translate = glm::vec3(1.0f, 2.0f, 3.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(0.0f)),
               glm::vec3(1.0f, 2.0f, 3.0f));
  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(1.0f, 1.0f, 1.0f)),
               glm::vec3(2.0f, 3.0f, 4.0f));
}

/**
 * The translation must live in the last column of the matrix.
 */
TEST(TransformComponentTest, Translation_FillsTranslationColumn) {
  Bored::TransformComponent transform;
  transform.translate = glm::vec3(4.0f, -5.0f, 6.0f);

  glm::mat4 matrix = transform.GetTransformMatrix();

  EXPECT_FLOAT_EQ(matrix[3][0], 4.0f);
  EXPECT_FLOAT_EQ(matrix[3][1], -5.0f);
  EXPECT_FLOAT_EQ(matrix[3][2], 6.0f);
  EXPECT_FLOAT_EQ(matrix[3][3], 1.0f);
}

///////////// Rotation //////////////

/**
 * 90 degrees around X: y -> z, z -> -y.
 */
TEST(TransformComponentTest, RotationX_90Degrees) {
  Bored::TransformComponent transform;
  transform.rotate.x = glm::radians(90.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(0.0f, 1.0f, 0.0f)),
               glm::vec3(0.0f, 0.0f, 1.0f));
  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(0.0f, 0.0f, 1.0f)),
               glm::vec3(0.0f, -1.0f, 0.0f));
}

/**
 * 90 degrees around Y: x -> -z, z -> x.
 */
TEST(TransformComponentTest, RotationY_90Degrees) {
  Bored::TransformComponent transform;
  transform.rotate.y = glm::radians(90.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(1.0f, 0.0f, 0.0f)),
               glm::vec3(0.0f, 0.0f, -1.0f));
  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(0.0f, 0.0f, 1.0f)),
               glm::vec3(1.0f, 0.0f, 0.0f));
}

/**
 * 90 degrees around Z: x -> y, y -> -x.
 */
TEST(TransformComponentTest, RotationZ_90Degrees) {
  Bored::TransformComponent transform;
  transform.rotate.z = glm::radians(90.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(1.0f, 0.0f, 0.0f)),
               glm::vec3(0.0f, 1.0f, 0.0f));
  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(0.0f, 1.0f, 0.0f)),
               glm::vec3(-1.0f, 0.0f, 0.0f));
}

/**
 * Rotation composes as Rz * Ry * Rx, i.e. the point is first rotated around X,
 * then Y, then Z. Under a 90/90/90 rotation the basis vectors must land at
 * x -> (0, 0, -1), y -> (0, 1, 0), z -> (1, 0, 0). An XYZ order would send
 * x -> (0, 0, 1) instead, so this test pins down the ZYX convention.
 */
TEST(TransformComponentTest, RotationZYX_Order) {
  Bored::TransformComponent transform;
  transform.rotate = glm::vec3(glm::radians(90.0f));

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(1.0f, 0.0f, 0.0f)),
               glm::vec3(0.0f, 0.0f, -1.0f));
  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(0.0f, 1.0f, 0.0f)),
               glm::vec3(0.0f, 1.0f, 0.0f));
  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(0.0f, 0.0f, 1.0f)),
               glm::vec3(1.0f, 0.0f, 0.0f));
}

///////////// Scale //////////////

/**
 * Uniform scale stretches every coordinate equally.
 */
TEST(TransformComponentTest, Scale_Uniform) {
  Bored::TransformComponent transform;
  transform.scale = glm::vec3(2.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(1.0f, 1.0f, 1.0f)),
               glm::vec3(2.0f, 2.0f, 2.0f));
}

/**
 * Non-uniform scale stretches each axis independently.
 */
TEST(TransformComponentTest, Scale_NonUniform) {
  Bored::TransformComponent transform;
  transform.scale = glm::vec3(2.0f, 3.0f, 4.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(1.0f, 1.0f, 1.0f)),
               glm::vec3(2.0f, 3.0f, 4.0f));

  glm::mat4 matrix = transform.GetTransformMatrix();
  EXPECT_FLOAT_EQ(matrix[0][0], 2.0f);
  EXPECT_FLOAT_EQ(matrix[1][1], 3.0f);
  EXPECT_FLOAT_EQ(matrix[2][2], 4.0f);
}

///////////// Composition //////////////

/**
 * T(1, 2, 3) * Rz(90) * S(2, 1, 1) maps (1, 0, 0) to (1, 4, 3).
 */
TEST(TransformComponentTest, Composition_TranslateRotateScale) {
  Bored::TransformComponent transform;
  transform.translate = glm::vec3(1.0f, 2.0f, 3.0f);
  transform.rotate = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));
  transform.scale = glm::vec3(2.0f, 1.0f, 1.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(1.0f, 0.0f, 0.0f)),
               glm::vec3(1.0f, 4.0f, 3.0f));
}

/**
 * T(1, 2, 3) * Rx(90) * S(1, 2, 1) maps (0, 1, 0) to (1, 2, 5).
 */
TEST(TransformComponentTest, Composition_AllComponents) {
  Bored::TransformComponent transform;
  transform.translate = glm::vec3(1.0f, 2.0f, 3.0f);
  transform.rotate = glm::vec3(glm::radians(90.0f), 0.0f, 0.0f);
  transform.scale = glm::vec3(1.0f, 2.0f, 1.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(0.0f, 1.0f, 0.0f)),
               glm::vec3(1.0f, 2.0f, 5.0f));
}

///////////// Edge cases //////////////

/**
 * A zero scale collapses every point onto the translation.
 */
TEST(TransformComponentTest, EdgeCase_ZeroScale) {
  Bored::TransformComponent transform;
  transform.translate = glm::vec3(5.0f, 6.0f, 7.0f);
  transform.scale = glm::vec3(0.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(1.0f, 2.0f, 3.0f)),
               glm::vec3(5.0f, 6.0f, 7.0f));
}

/**
 * A negative scale mirrors the affected axes.
 */
TEST(TransformComponentTest, EdgeCase_NegativeScale) {
  Bored::TransformComponent transform;
  transform.scale = glm::vec3(-1.0f, 1.0f, 1.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(1.0f, 2.0f, 3.0f)),
               glm::vec3(-1.0f, 2.0f, 3.0f));
}

/**
 * 180 degrees around X flips y and z.
 */
TEST(TransformComponentTest, EdgeCase_Rotation180Degrees) {
  Bored::TransformComponent transform;
  transform.rotate.x = glm::radians(180.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(0.0f, 1.0f, 0.0f)),
               glm::vec3(0.0f, -1.0f, 0.0f));
  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(0.0f, 0.0f, 1.0f)),
               glm::vec3(0.0f, 0.0f, -1.0f));
}

/**
 * A full 360 degree turn must reproduce the identity matrix. A small
 * tolerance is required because 2 * pi is not exactly representable in float.
 */
TEST(TransformComponentTest, EdgeCase_Rotation360Degrees) {
  Bored::TransformComponent transform;
  transform.rotate = glm::vec3(glm::radians(360.0f));

  ExpectMatrixEq(transform.GetTransformMatrix(), glm::mat4(1.0f));
}

/**
 * Because the matrix is T * R * S, scaling happens before rotation. Under
 * Rz(90) * S(2, 1, 1) the point (1, 0, 0) lands at (0, 2, 0); with scale
 * applied after rotation it would land at (0, 1, 0).
 */
TEST(TransformComponentTest, EdgeCase_ScaleAppliedBeforeRotation) {
  Bored::TransformComponent transform;
  transform.rotate = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));
  transform.scale = glm::vec3(2.0f, 1.0f, 1.0f);

  ExpectVec3Eq(Apply(transform.GetTransformMatrix(), glm::vec3(1.0f, 0.0f, 0.0f)),
               glm::vec3(0.0f, 2.0f, 0.0f));
}
