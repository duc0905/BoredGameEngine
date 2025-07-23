#pragma once
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <memory>
#include "../ECS/Node.hpp"

namespace Bored {
/**
 * This class generates the projection-matrices.
 *
 * Projection-matrices are used to convert camera coordinates to image
 * coordinates.
 */
class Projector {
public:
  virtual ~Projector() {}

  /**
   * Get the projection-matrix
   *
   * Get the 4x4 projection matrix to convert camera coordinates to image
   * coodinates. The image plane is in (-1, 1) for both x and y components.
   *
   * @return glm::mat4 the projection-matrix
   */
  [[nodiscard]] virtual glm::mat4 GetProjectionMatrix() const = 0;
};

/**
 * This class generates the orthographic projection matrix.
 */
class Orthographic : public Projector {
public:
  /**
   * Full constructor for Orthographic.
   *
   * Has all the options for constructing Orthographic.
   *
   * @param left left-bound for the projection plane in world coordinates.
   *
   * @param right right-bound for the projection plane in world coordinates.
   *
   * @param bottom bottom-bound for the projection plane in world coordinates.
   *
   * @param top top-bound for the projection plane in world coordinates.
   *
   * @param near near-bound for the projection plane in world coordinates.
   *
   * @param far far-bound for the projection plane in world coordinates.
   *
   */
  Orthographic(float left, float right, float bottom, float top, float near,
               float far)
      : m_left(left), m_right(right), m_bottom(bottom), m_top(top),
        m_near(near), m_far(far) {}

  /**
   * Get projection matrix.
   *
   * Get the orthographic projection matrix.
   *
   * @return glm::mat4 a 4x4 matrix for orthographic projection
   *
   */
  virtual glm::mat4 GetProjectionMatrix() const {
    return glm::orthoLH_ZO(m_left, m_right, m_bottom, m_top, m_near, m_far);
  }

private:
  float m_left;   /**< Left-bound for projection plane */
  float m_right;  /**< Right-bound for projection plane */
  float m_bottom; /**< Bottom-bound for projection plane */
  float m_top;    /**< Top-bound for projection plane */
  float m_near;   /**< Near-bound for projection plane */
  float m_far;    /**< Top-bound for projection plane */
};

/**
 * This class generates the perspective projection matrix.
 */
class Perspective : public Projector {
public:
  /**
   * Default constructor.
   *
   * Use FOV = pi/2, width = height = 600.0f, and near = 1.0f.
   */
  Perspective() : Perspective(glm::pi<float>() / 2, 600.0f, 600.0f, 1.0f) {}

  /**
   * Full constructor for Perspective.
   *
   * Has all the options for constructing Orthographic.
   *
   * @param fov the field of view for the perspective.
   *
   * @param width the width of the image plane
   *
   * @param height the height of the image plane
   *
   * @param near the near-bound to clip
   *
   */
  Perspective(float fov, float width, float height, float near)
      : m_fov(fov), m_aspect(width / height), m_f(near) {}

  /**
   * Get projection matrix.
   *
   * Get the perspective projection matrix.
   *
   * @return glm::mat4 a 4x4 matrix for perspective projection
   *
   */
  [[nodiscard]] virtual glm::mat4 GetProjectionMatrix() const {
    return glm::infinitePerspective(m_fov, m_aspect, m_f);
  }

  /**
   * Helper function to get focal length given fov and sensor width.
   *
   * @param fov The field of view in radians.
   *
   * @param d The sensor width.
   * @note Be consistent with the unit for d and f with other parts of your
   * code.
   * @sa GetFOV
   */
  [[nodiscard]] static float GetFocalLength(float fov, float d) {
    return d / (2.0f * glm::tan(fov / 2.0f));
  }

  /**
   * Helper function to get fov given focal length and sensor width.
   *
   * @param f The focal length.
   *
   * @param d The sensor width.
   *
   * @note d and f should be using the same length unit.
   * @note Be consistent with the unit for d and f with other parts of your
   * code.
   * @sa GetFocalLength
   */
  [[nodiscard]] static float GetFOV(float f, float d) {
    return 2.0f * glm::atan(d / (2.0f * f));
  }

public:
  float m_f;      /**< Focal length */
  float m_fov;    /**< Field of view of the camera */
  float m_aspect; /**< Aspect ratio of the image plane (width/height) */
};

/**
 * Camera abstraction.
 *
 * Contains a View and a Projector to abstract the concept of a camera.
 */
class Camera : public Node {
public:
  /**
   * Default constructor.
   *
   * Use default View and default Perspective.
   */
  Camera() : Camera(new View(), new Perspective()) {}

  /**
   * Full constructor for Camera
   *
   * @param view View*
   *
   * @param proj Projector*
   */
  Camera(View *view, Projector *proj) : m_view(view), m_proj(proj) {}

  [[nodiscard]] glm::mat4 GetViewMatrix() const {
    return glm::inverse(transform.GetTransformMatrix());
  }

  /**
   * Get the view-projection matrix.
   *
   * Combine the view and projection matrix from the camera's View and
   * Projector.
   *
   */
  [[nodiscard]] glm::mat4 GetViewProjectionMatrix() const {
    return m_proj->GetProjectionMatrix() * GetViewMatrix();
  }

public:
  std::unique_ptr<Projector> m_proj;
};

} // namespace Bored
