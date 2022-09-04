#pragma once

#include "IGame.h"
#include "HUD/ImGuiHUD.h"
#include "HUD/HUDComponents/ImGuiTextEl.h"

void ImGuiHUDDemo()
{
	auto input = IGame::GetInputPtr();
	auto hud = std::make_shared<ImGuiHUD>();
	IGame::SetHUD(hud);

	auto& wi = dynamic_cast<ImGuiHUDWindow&>(hud->AddWindow());

	wi.SetTitle("abcd");
	auto* textEl = new ImGuiTextEl("I am %s", "duc0905");
	wi.AddElement(textEl);

	std::shared_ptr<Context> con = std::make_shared<Context>();
	con->AddActionMapping(KeyInput::KEY_T, 0, "CHANGE_NAME");
	input->AddContext(con);
	input->ActivateContext(con);

	// Refer variables as values ('=' symbol) because when 
	// the game runs, all these variables are 
	// out of scope so cannot be refered as reference, but
	// textEl is a pointer so it's okay to refer as value
	input->BindAction("CHANGE_NAME", [=](KeyInput::Action action) -> void {
		if (action == KeyInput::PRESS)
			textEl->ChangeParams("not duc");
		else if (action == KeyInput::RELEASE)
			textEl->ChangeParams("duc0905");
		});


}
