#pragma once
#ifndef OAL_MODS_H
#include "Core.h"

namespace Bored {
namespace OAL {

class Audio : public Bored::Audio {
  // Inherited via Audio
  virtual void OnSetup() override {}
};
};  // namespace OAL
};  // namespace Bored
#endif  // !OAL_MODS_H
