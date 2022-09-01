#pragma once
#include "../pch.h"
#include "../System.h"

class IHUD : public System
{
	static std::shared_ptr<IHUD> defaultHUD_;
};

class NullHUD : public IHUD
{};

std::shared_ptr<IHUD> IHUD::defaultHUD_ = std::make_shared<NullHUD>();
