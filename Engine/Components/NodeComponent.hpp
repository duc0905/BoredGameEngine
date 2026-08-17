#pragma once
#include <memory>
#include <set>

namespace Bored {
class Object;

/**
 * Store the tree hirarchical information.
 *
 * NodeComponent stores the tree hierarchy information. Use this component to
 * get tree hierarchy when needed, for example for global transformation.
 */
struct NodeComponent {
  std::shared_ptr<Object> self;
  std::shared_ptr<Object> parent;
  std::set<std::shared_ptr<Object>> children;
};
}  // namespace Bored
