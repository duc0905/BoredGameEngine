#include "Factory.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

namespace Bored
{
namespace Render
{
namespace OGL
{

std::shared_ptr<IMesh> Factory::CreateMesh()
{
    return std::make_shared<Mesh>();
}

std::shared_ptr<IMesh> Factory::CreateMesh(IMesh& other)
{
    return std::make_shared<Mesh>(other);
}

std::shared_ptr<Material> Factory::CreateMaterial()
{
    return std::make_shared<Material>();
}
std::shared_ptr<Material> Factory::CreateMaterial(Material& other)
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

std::shared_ptr<ITexture> Factory::CreateTexture()
{
    return std::make_shared<Texture>();
}

std::shared_ptr<ITexture> Factory::CreateTexture(ITexture& other)
{
    return std::make_shared<Texture>(other);
}

} // namespace OGL
} // namespace Render
} // namespace Bored
