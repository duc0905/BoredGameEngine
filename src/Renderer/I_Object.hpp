#pragma once
#include <glm/glm.hpp>
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
  std::shared_ptr<I_Object3D> obj =
      nullptr; /**< The intersected object. If does not intersect, this field is
                  nullptr */
  glm::vec3 n; /**< The normal at intersection point */
  glm::vec3 p; /**< The point at intersection */
  double lambda = 0.0;     /**< Distance to the intersection. */
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
  //////////////// Transformation //////////////////
  /**
   * Rotate the object around X-axis.
   *
   * @param theta The angle to rotate in radians.
   */
  I_Object3D &RotateX(double theta);

  /**
   * Rotate the object around Y-axis.
   *
   * @param theta The angle to rotate in radians.
   */
  I_Object3D &RotateY(double theta);

  /**
   * Rotate the object around Z-axis.
   *
   * @param theta The angle to rotate in radians.
   */
  I_Object3D &RotateZ(double theta);

  /**
   * Translate the object.
   *
   * @param t The distance to translate the object by.
   */
  I_Object3D &Translate(const glm::vec3 &t);

  /**
   * Scale the object.
   *
   * @param s The amount to scale the object by for each of the 3 axis.
   */
  I_Object3D &Scale(const glm::vec3 &s);

protected:
  glm::mat4 T = glm::mat4(1.0f); /**< Transformation matrix of the object,
                  containing the transformation from its canonical form. */
  glm::mat4 Tinv =
      glm::mat4(1.0f); /**< Inverse of the transformation matrix. */
};

/**
 * Interface to define a ray tracing object.
 */
class I_RTObject : public I_Object3D {
public:
  /**
   * Intersection test for the ray and the current object.
   *
   * @sa Intersection
   *
   * @sa Ray3D
   */
  virtual Intersection Intersect(const Ray3D &ray) = 0;
};
