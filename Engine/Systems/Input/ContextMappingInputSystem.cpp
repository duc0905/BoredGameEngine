#include "ContextMappingInputSystem.hpp"

#include <stdexcept>

namespace Bored {
ContextMappingInputSystem::ContextMappingInputSystem(IOService& io_service)
    : InputSystem(io_service) {};

void ContextMappingInputSystem::HandleInputEvents(
    std::vector<InputEvent>& events) {
  // Get active context
  if (active_ctx_idx >= contexts.size())
    throw std::runtime_error("active input context index out of bound");

  InputCtx& context = contexts[active_ctx_idx];

  for (auto& e : events) {
    if (context.itoa_map.contains(e.type)) {
      auto& action = context.itoa_map[e.type];

      if (context.atoh_map.contains(action)) {
        auto& handler = context.atoh_map[action];
        handler(e);
      }
    }  // else: physical input not mapped in this context.
  }
}
}  // namespace Bored
