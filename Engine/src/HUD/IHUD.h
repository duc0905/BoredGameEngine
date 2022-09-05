#pragma once
#include "../pch.h"
#include "../System.h"
#include "HUDComponents/IHUDWindow.h"

class IHUD : public System
{
private:
	static std::shared_ptr<IHUD> defaultHUD_;
public:
	static std::shared_ptr<IHUD> GetDefault() { return defaultHUD_; }

	virtual IHUDWindow& AddWindow() = 0;
};

class NullHUD : public IHUD
{
private:
	NullHUDWindow nullWindow_;
public:
	virtual IHUDWindow& AddWindow() override { return nullWindow_; }
};
