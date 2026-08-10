#pragma once
#include "../Scene/Node.hpp"

namespace Bored {
/**
 * Store the pointer to the Node.
 *
 * Node stores the tree hierarchy information. Use this component to get tree
 * hierarchy when needed, for example for global transformation.
 */
struct NodeComponent {
  std::shared_ptr<Node> node;
};
} // namespace Bored
