#pragma once
#include "../RenderUtil.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

namespace Bored
{
namespace Render
{
namespace CPU
{

class Factory : public IFactory
{
  public:
    virtual std::shared_ptr<IMesh> CreateMesh()
    {
        return std::make_shared<Mesh>();
    }
    virtual std::shared_ptr<IMesh> CreateMesh(IMesh& other)
    {
        return std::make_shared<Mesh>(other);
    }
    virtual std::shared_ptr<Material> CreateMaterial()
    {
        return std::make_shared<Material>();
    }
    virtual std::shared_ptr<Material> CreateMaterial(Material& other)
    {
        auto mat = CreateMaterial();
        mat->name = other.name;
        mat->opacity = other.opacity;

        if (other.diffuse != nullptr)
            mat->diffuse = CreateTexture(*other.diffuse);
        if (other.specular != nullptr)
            mat->specular = CreateTexture(*other.specular);

        return mat;
    }
    virtual std::shared_ptr<ITexture> CreateTexture()
    {
        return std::make_shared<Texture>();
    }
    virtual std::shared_ptr<ITexture> CreateTexture(ITexture& other)
    {
        return std::make_shared<Texture>(other);
    }
};

} // namespace CPU
} // namespace Render
} // namespace Bored
