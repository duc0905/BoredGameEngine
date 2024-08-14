#pragma once
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace Bored
{
namespace ECS
{

/**
 * TODO: Document
 *
 * Description.
 *
 * @param name Type and description of the parameter.
 * @return Type and description of the returned value.
 *
 * @example
 * // Description of my example.
 * Write me later
 */
class Projector
{
  public:
    virtual ~Projector()
    {
    }

    // Get the projection matrix for this projector
    [[nodiscard]] virtual glm::mat4 GetMat() const = 0;
};

/**
 * TODO: Document
 *
 * Description.
 *
 * @param name Type and description of the parameter.
 * @return Type and description of the returned value.
 *
 * @example
 * // Description of my example.
 * Write me later
 */
class OrthoProjector : public Projector
{
    float l, r, t, b;

  public:
    OrthoProjector(float left, float right, float bottom, float top) : l(left), r(right), t(top), b(bottom)
    {
    }

    [[nodiscard]] virtual glm::mat4 GetMat() const override
    {
        return glm::ortho(l, r, b, t);
    }
};

/**
 * TODO: Document
 *
 * Description.
 *
 * @param name Type and description of the parameter.
 * @return Type and description of the returned value.
 *
 * @example
 * // Description of my example.
 * Write me later
 */
class PerspProjector : public Projector
{
  public:
    const unsigned int &w, h;
    float fov = 30.0f;
    float zNear = 0.1f, zFar = 100.0f;

  public:
    /**
     * TODO: A one-line summary.
     *
     * Description.
     */
    PerspProjector(const unsigned int& width, const unsigned int& height) : w(width), h(height)
    {
    }

    [[nodiscard]] virtual glm::mat4 GetMat() const override
    {
        return glm::perspectiveFov(fov, (float)w, (float)h, zNear, zFar);
    }
};
/**
 * TODO: Document
 *
 * Description.
 *
 * @param name Type and description of the parameter.
 * @return Type and description of the returned value.
 *
 * @example
 * // Description of my example.
 * Write me later
 */
struct Camera
{
    Camera(Projector* p_projector = new OrthoProjector(0.0f, 100.0f, 0.0f, 100.0f)) : projector(std::move(p_projector))
    {
    }

    glm::vec3 dir = {1.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 0.0f, 1.0f};
    float yaw = 0.0f, pitch = 0.0f;

    std::unique_ptr<Projector> projector;
};

} // namespace ECS
} // namespace Bored
