#pragma once

#include "Node.hpp"

namespace Bored {
/**
 * Represents a world/level in the game
 *
 * Containing information specifically to a world/level such as nodes, input
 * mapping, etc.
 *
 * @example
 * // Description of my example.
 * Write me later
 */
class Scene {
public:
  Scene() {}

private:
  std::shared_ptr<Node> root;
};
} // namespace Bored
