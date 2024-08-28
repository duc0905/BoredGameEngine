#pragma once
#include "../RenderUtil.hpp"

namespace Bored
{
namespace Render
{
namespace OGL
{
class Factory : public Render::IFactory
{
  public:
    virtual std::shared_ptr<IMesh> CreateMesh() override;
    virtual std::shared_ptr<IMesh> CreateMesh(IMesh& other) override;

    virtual std::shared_ptr<Material> CreateMaterial() override;
    virtual std::shared_ptr<Material> CreateMaterial(Material& other) override;

    virtual std::shared_ptr<ITexture> CreateTexture() override;
    virtual std::shared_ptr<ITexture> CreateTexture(ITexture& other) override;
};
} // namespace OGL
} // namespace Render
} // namespace Bored
