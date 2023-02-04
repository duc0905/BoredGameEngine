#include "ChessGameMode.h"
#include "IGame.h"

ChessGameMode::ChessGameMode(IWorld& world)
	: IGameMode(world)
{	
	auto& hud = IGame::GetHUD();
	IHUDWindow& debugWindow = hud.AddWindow();
	ImGuiHUDWindow& debug = static_cast<ImGuiHUDWindow&>(debugWindow);
	debug.SetTitle("Debug Window");

	text = new ImGuiTextEl("0");
	debug.AddElement(text);

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			ChessBoardState state;
			if (y == 1) {
				state.team = 0;
				state.type = PissActor::Type::PAWN;
				currentChessBoardState.insert(currentChessBoardState.begin() + x + y * 8, state);
			}
		}
	}
}

void ChessGameMode::OnTick(float)
{
	//LOG_COLOR("Chess on tick", COLOR::BLUE, COLOR::BLACK);
	auto& input = IGame::GetInput();
	int actorID = input.GetCursorHoveringActor().GetID();
	auto stringID = std::to_string(actorID);
	text->updateString("Current actor by ID: " + stringID);
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