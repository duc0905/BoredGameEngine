#pragma once
#include "../pch.h"

namespace Bored {
struct Camera {
  glm::vec3 dir = {1.0f, 0.0f, 0.0f};
  glm::vec3 up = {0.0f, 0.0f, 1.0f};
  float yaw = 0.0f, pitch = 0.0f;

  [[nodiscard]] glm::vec3 GetDir() const;
  [[nodiscard]] glm::mat4 GetViewMat(const glm::vec3& pos) const;
};

class Projector
{
public:
    [[nodiscard]] virtual glm::mat4 GetMat() const = 0;
};

class OrthoProjector : public Projector
{
    float l, r, t, b;
public:
    OrthoProjector(float left, float right, float bottom, float top)
        : l(left), r(right), t(top), b(bottom) {}
    virtual glm::mat4 GetMat() const override;
};

class PerspProjector : public Projector
{
public:
    const unsigned int& w, h;
    float fov = 30.0f;
    float zNear = 0.1f, zFar = 100.0f;
public:
    PerspProjector(const unsigned int& width, const unsigned int& height)
        : w(width), h(height) {}
    virtual glm::mat4 GetMat() const override;
};
}
