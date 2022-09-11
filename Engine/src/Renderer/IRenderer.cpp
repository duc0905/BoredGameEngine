#include "pch.h"
#include "IRenderer.h"

std::shared_ptr<IRenderer> IRenderer::defaultRenderer_ = std::make_shared<NullRenderer>();