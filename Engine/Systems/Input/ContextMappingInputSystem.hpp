#pragma once

#include <map>

#include "InputSystem.hpp"
#include "../../Systems/Input/IOService.hpp"

namespace Bored {
/**
 * Context-based input-to-action mapping system.
 *
 * A scene can have multiple contexts, such as main gameplay, inventory menu,
 * settings menu, etc. In each such context, the input handling is different.
 *
 * For example, pressing A in min gameplay context should translate to the
 * character moving to the left, but in the inventory context, pressing a
 * triggers the menu shortcut for sorting items in ascending order.
 *
 * Only the active context is used. If the input event is not handled, the event
 * is not passed to any other context. This system can and should be used in a
 * scene with many contexts.
 *
 * @todo Figure out how to integrate input system into an event system
 * seemlessly.
 *
 * @example
 * // Description of my example.
 * @todo Write example.
 */
class ContextMappingInputSystem : public InputSystem {
 public:
  ContextMappingInputSystem(IOService& io_service);
  virtual void HandleInputEvents(std::vector<InputEvent>& events) override;

  void SwitchContext(unsigned int ctx_id);

 private:
  // Mapping physical event to action name
  typedef std::map<InputType, std::string> InputActionMap;

  // Mapping action to function handler
  // @todo Debate whether InputEvent should be passed to function handler
  // everytime.
  typedef std::map<std::string, std::function<void(InputEvent&)>>
      ActionHandlerMap;

  struct InputCtx {
    InputActionMap itoa_map;
    ActionHandlerMap atoh_map;
  };

  std::vector<InputCtx> contexts;
  unsigned int active_ctx_idx = 0;
};
}  // namespace Bored
