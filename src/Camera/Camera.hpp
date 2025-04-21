#pragma once
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <memory>

/**
 * This class generates the view-matrices.
 *
 * View-matrices are used to convert world coordinates to camera coordinates.
 */
class View {
public:
  /**
   * Full constructor for View.
   *
   * Has all the options for constructing View.
   *
   * @param pos position of the camera in world coordinates.
   *
   * @param dir view direction of the camera.
   *
   * @param up the up vector of the camera.
   *
   */
  View(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up)
      : m_pos(pos), m_dir(dir), m_up(up) {}

  /**
   * Get view matrix.
   *
   * Get a 4x4 matrix for converting world coordinates to camera coordinates.
   */
  [[nodiscard]] glm::mat4 GetViewMatrix() const {
    return glm::lookAt(m_pos, m_pos + m_dir, m_up);
  }

private:
  glm::vec3 m_pos; /**< Camera position */
  glm::vec3 m_dir; /**< Camera view direction */
  glm::vec3 m_up;  /**< Camera up vector */
};

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
   * @param left left-bound for the projection plane.
   *
   * @param right right-bound for the projection plane.
   *
   * @param bottom bottom-bound for the projection plane.
   *
   * @param top top-bound for the projection plane.
   *
   * @param near near-bound for the projection plane.
   *
   * @param far far-bound for the projection plane.
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
   * @param far the far-bound to clip
   *
   */
  Perspective(float fov, float width, float height, float near, float far)
      : m_fov(fov), m_aspect(width / height), m_near(near), m_far(far) {}

  /**
   * Get projection matrix.
   *
   * Get the perspective projection matrix.
   *
   * @return glm::mat4 a 4x4 matrix for perspective projection
   *
   */
  [[nodiscard]] virtual glm::mat4 GetProjectionMatrix() const {
    return glm::perspective(m_fov, m_aspect, m_near, m_far);
  }

private:
  float m_fov; /**< Field of view of the camera */
  float m_aspect; /**< Aspect ratio of the image plane (width/height) */
  float m_near; /**< Near bound to clip */
  float m_far; /**< Far bound to clip*/
};

/**
 * Camera abstraction.
 *
 * Contains a View and a Projector to abstract the concept of a camera.
 */
class Camera {
public:
  /**
   * Full constructor for Camera
   *
   * @param view View*
   *
   * @param proj Projector*
   */
  Camera(View *view, Projector *proj) : m_view(view), m_proj(proj) {}

  /**
   * Get the view-projection matrix.
   *
   * Combine the view and projection matrix from the camera's View and Projector.
   *
   */
  [[nodiscard]] glm::mat4 GetViewProjectionMatrix() const {
    return m_proj->GetProjectionMatrix() * m_view->GetViewMatrix();
  }

protected:
  std::unique_ptr<View> m_view;
  std::unique_ptr<Projector> m_proj;
};
