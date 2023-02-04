#include "ChessGameMode.h"
#include "IGame.h"

ChessGameMode::ChessGameMode(IWorld& world)
	: IGameMode(world)
{	
	auto& hud = IGame::GetHUD();
	IHUDWindow& debugWindow = hud.AddWindow();
	ImGuiHUDWindow& debug = static_cast<ImGuiHUDWindow&>(debugWindow);
	debug.SetTitle("Debug Window");

	text = std::make_unique<ImGuiTextEl>("ID: %d", 0);
	debug.AddElement(text.get());
}

void ChessGameMode::OnTick(float)
{
	//LOG_COLOR("Chess on tick", COLOR::BLUE, COLOR::BLACK);
	auto& input = IGame::GetInput();
	unsigned int actorID = 1;
		//input.GetCusorHoveringActor().GetID();
	text->ChangeParams(actorID);
	//LOG(input.GetCusorHoveringActor().GetID());
}

void ChessGameMode::OnLevelStart()
{
	LOG_COLOR("Chess on level start", COLOR::BLUE, COLOR::BLACK);
}

void ChessGameMode::OnLevelPause()
{
	LOG_COLOR("Chess on pause", COLOR::BLUE, COLOR::BLACK);
}

void ChessGameMode::OnLevelResume()
{
	LOG_COLOR("Chess on resume", COLOR::BLUE, COLOR::BLACK);
}

void ChessGameMode::OnLevelEnd()
{
	LOG_COLOR("Chess on end", COLOR::BLUE, COLOR::BLACK);
}

void ChessGameMode::OnAttach(IWorld& world)
{
	LOG_COLOR("Chess on attach", COLOR::BLUE, COLOR::BLACK);
}

void ChessGameMode::OnDetach()
{
	LOG_COLOR("Chess on detach", COLOR::BLUE, COLOR::BLACK);
}