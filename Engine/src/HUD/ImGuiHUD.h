#pragma once
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "IHUD.h"
#include  "HUDComponents/ImGuiWindow.h"

class IGame;

class ImGuiHUD : public IHUD
{
private:
	std::vector<std::unique_ptr<ImGuiHUDWindow>> windows_;
public:
	ImGuiHUD();
	~ImGuiHUD();

	virtual void OnTick(float dt) override;

	// Inherited via IHUD
	virtual IHUDWindow& AddWindow() override;
};