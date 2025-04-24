#pragma once

#include "../I_Object.hpp"
#include <glm/exponential.hpp>
#include <iostream>

/**
 * Represents a canonical sphere, center at origin and radius 1.
 */
class Sphere : public I_RTObject {
protected:
  /**
   * Interection test the ray with the canonical sphere.
   *
   * @param ray The ray converted to model coordinates.
   *
   * @return The intersection with respect to the canonical sphere.
   */
  virtual Intersection IntersectCanonical(const Ray3D &ray) {
    //    (x + l * dx)^2 + (y + l * dy)^2 + (z + l * dz)^2 - 1 = 0
    // => x2 + y2 + z2 + 2*l*(xdx + ydy + zdz) + l^2 * (dx2 + dy2 + dz2) - 1 = 0
    // => l^2 * (d . d) + 2*l*(p . d) + (p . p - 1) = 0

    glm::vec3 p = glm::vec3(ray.m_origin);
    glm::vec3 d = glm::vec3(ray.m_dir);
    float A = glm::dot(d, d);
    float B = 2.0f * glm::dot(p, d);
    float C = glm::dot(p, p) - 1.0f;
    float D = B * B - 4.0f * A * C;

    if (D < 0.0f)
      return {};
    else {
      float sqrtD = glm::sqrt(D);
      float lambda = (-B - sqrtD) / (2.0f * A);

      if (lambda >= 0.0f) {
        glm::vec3 p = ray.m_origin + lambda * ray.m_dir;
        return {
            .obj = this, .lambda = lambda, .n{p}, .p{p}, .u = 0.0f, .v = 0.0f};
      }

      lambda = (-B + sqrtD) / (2.0f * A);
      if (lambda >= 0.0f) {
        glm::vec3 p = ray.m_origin + lambda * ray.m_dir;
        return {
            .obj = this, .lambda = lambda, .n{p}, .p{p}, .u = 0.0f, .v = 0.0f};
      }

      return {};
    }
  }
};
