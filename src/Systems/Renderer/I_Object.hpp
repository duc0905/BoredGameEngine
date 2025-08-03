#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <memory>

struct Ray3D {
  glm::vec4 m_origin;
  glm::vec4 m_dir;
};

// Forward declration
class I_Object3D;

/**
 * Contains the information for intersections.
 *
 * Mostly used in ray tracing.
 */
struct Intersection {
  I_Object3D *obj = nullptr; /**< The intersected object. If does not intersect,
                                this field is nullptr */
  double lambda = 0.0;       /**< Distance to the intersection. */
  glm::vec3 n;               /**< The normal at intersection point */
  glm::vec3 p;               /**< The point at intersection */
  double u = 0.0, v = 0.0; /**< UV-coordinates for texture and other mappings */
};

/**
 * Interface to define an object in the 3D world.
 *
 * Including static objects, movable objects, non-visible objects, light
 * objects, ...
 */
class I_Object3D {
public:
  virtual ~I_Object3D() {}

  //////////////// Transformation //////////////////
  /**
   * Rotate the object around X-axis.
   *
   * @param theta The angle to rotate in radians.
   */
  I_Object3D &RotateX(float theta) {
    m_T = glm::rotate(glm::mat4(1.0f), theta, {1.0f, 0.0f, 0.0f}) * m_T;
    return *this;
  }

  /**
   * Rotate the object around Y-axis.
   *
   * @param theta The angle to rotate in radians.
   */
  I_Object3D &RotateY(float theta) {
    m_T = glm::rotate(glm::mat4(1.0f), theta, {0.0f, 1.0f, 0.0f}) * m_T;
    return *this;
  }

  /**
   * Rotate the object around Z-axis.
   *
   * @param theta The angle to rotate in radians.
   */
  I_Object3D &RotateZ(float theta) {
    m_T = glm::rotate(glm::mat4(1.0f), theta, {0.0f, 0.0f, 1.0f}) * m_T;
    return *this;
  }

  /**
   * Translate the object.
   *
   * @param t The distance to translate the object by.
   */
  I_Object3D &Translate(const glm::vec3 &t) {
    m_T = glm::translate(glm::mat4(1.0f), t) * m_T;
    return *this;
  }

  /**
   * Scale the object.
   *
   * @param s The amount to scale the object by for each of the 3 axis.
   */
  I_Object3D &Scale(const glm::vec3 &s) {
    m_T = glm::scale(glm::mat4(1.0f), s) * m_T;
    return *this;
  }

  void Finalize() { m_Tinv = glm::inverse(m_T); }

public:
  glm::mat4 m_T = glm::mat4(1.0f); /**< Transformation matrix of the object,
                  containing the transformation from its canonical form. */
  glm::mat4 m_Tinv =
      glm::mat4(1.0f); /**< Inverse of the transformation matrix. */
};

/**
 * Interface to define a ray tracing object.
 */
class I_RTObject : public I_Object3D {
public:
  virtual ~I_RTObject() {}

  [[nodiscard]] Ray3D W2M_Ray(const Ray3D &ray) const {
    return {.m_origin = m_Tinv * ray.m_origin, .m_dir = m_Tinv * ray.m_dir};
  }

  [[nodiscard]] Intersection Intersect(const Ray3D &ray) {
    // Convert ray
    Ray3D ray_m = W2M_Ray(ray);

    // Canonical object intersection
    Intersection in = IntersectCanonical(ray_m);

    // Convert intersection into world space
    if (!in.obj) // Does not find intersection
      return in;

    // Normal and intersection point
    in.n = glm::normalize(m_T * glm::vec4(in.n, 0.0f));
    in.p = m_T * glm::vec4(in.p, 1.0f);

    return in;
  }

protected:
  /**
   * Intersection test for the ray in Model coordinates and the canonical
   * object.
   *
   * @param ray The ray in model coordinates.
   *
   * @return The intersection with respect to the canonical object.
   *
   * @sa Intersection
   *
   * @sa Ray3D
   */
  [[nodiscard]] virtual Intersection IntersectCanonical(const Ray3D &ray) = 0;

public:
  glm::vec3 m_color;
  struct {
    struct {
      float ra; /**< Ambient light albedo */
      float rd; /**< Diffuse component albedo */
      float rs; /**< Specular component albedo */
      float rg; /**< Global component albedo */
      float rt; /**< Refraction component albedo */
    } m_albedo; /**< Albedo properties */

    float alpha; /**< Opacity */
    float r_index; /**< Refraction index */
    float shinyness; /**< idk... */
  } m_material; /**< Phong material of the object */
};
