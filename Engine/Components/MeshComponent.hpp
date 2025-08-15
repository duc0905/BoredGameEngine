#pragma once

#include "Mesh3D.hpp"
#include <memory>

namespace Bored {
struct MeshComponent {
  // TODO: Find out how to handle "resource" fields for the meta system.
  std::shared_ptr<ArrayMesh> mesh;
  std::shared_ptr<Material> material;
};
} // namespace Bored
