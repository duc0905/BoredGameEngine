#pragma once
#include "World/GameMode/IGameMode.h"
#include "HUD/HUDComponents/ImGuiTextEl.h"
#include "HUD/HUDComponents/ImGuiWindow.h"
#include "PissActor.h"
#include "TileActor.h"

class ChessGameMode : public IGameMode
{
	struct ChessBoardState {
		int team;
		PissActor::Type type;
		ChessBoardState() {
			team = -1; // Not occupied
			type = PissActor::EMPTY;
		}
	};
private:
	std::shared_ptr<PissActor> currentSelectedActor;
	std::vector<std::pair<int, int>> nextMove;
	std::vector<ChessBoardState> boardState;
	// Inherited via IGameMode
	
	std::vector<std::pair<int, int>> getPossibleMove(std::shared_ptr<PissActor> actor);

	virtual void OnLevelStart() override;
	virtual void OnLevelPause() override;
	virtual void OnLevelResume() override;
	virtual void OnLevelEnd() override;

	virtual void OnAttach(IWorld& world) override;
	virtual void OnDetach() override;

	virtual void OnTick(float) override;
public:
	ImGuiTextEl* text;
	ChessGameMode(IWorld& world);
};

