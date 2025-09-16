#pragma once

#include "Mesh3D.hpp"
#include <memory>

namespace Bored {
struct MeshComponent {
  std::shared_ptr<ArrayMesh> mesh;

  std::shared_ptr<Material> material;
};
} // namespace Bored
