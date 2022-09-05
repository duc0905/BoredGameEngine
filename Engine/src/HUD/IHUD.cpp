#include "pch.h"
#include "IHUD.h"

std::shared_ptr<IHUD> IHUD::defaultHUD_ = std::make_shared<NullHUD>();
