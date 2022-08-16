#include "../pch.h"
#include "IWorld.h"

std::shared_ptr<IWorld> IWorld::defaultWorld_ = std::make_shared<NullWorld>();