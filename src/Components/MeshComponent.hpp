#pragma once

#include <memory>
#include "Mesh3D.hpp"

namespace Bored {
struct MeshComponent {
  std::shared_ptr<ArrayMesh> mesh;
};
} // namespace Bored
