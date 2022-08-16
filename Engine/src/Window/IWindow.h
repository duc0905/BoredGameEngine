#pragma once

#include "../pch.h"
#include "../System.h"

class IWindow : public System
{
	static std::shared_ptr<IWindow> defaultWindow_;
public:
	static std::shared_ptr<IWindow> GetDefault()
	{
		return defaultWindow_;
	}

	virtual void OnTick(float) override = 0;
	virtual ~IWindow() {};

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;
	virtual bool ShouldClose() const = 0;
	virtual const std::string& GetTitle() const = 0;
};

class NullWindow : public IWindow
{
	// Inherited via IWindow
	virtual unsigned int GetWidth() const override { return 0; };
	virtual unsigned int GetHeight() const override { return 0; };
	virtual bool ShouldClose() const override { return true; };
	virtual const std::string& GetTitle() const override { return ""; };
	virtual void OnTick(float) override {};
};

