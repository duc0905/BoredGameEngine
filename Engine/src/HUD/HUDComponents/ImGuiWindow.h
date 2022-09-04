#pragma once
#include "IHUDWindow.h"
#include <imgui.h>

class ImGuiHUDWindow : public IHUDWindow
{
private:
	std::string title_;
public:
	// Inherited via IHUDWindow
	virtual bool Render() override;

	void SetTitle(const std::string& title) { title_ = title; }
	const std::string& GetTitle() const { return title_; }
};

