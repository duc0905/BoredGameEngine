#pragma once
#include "World/GameMode/IGameMode.h"
#include "HUD/HUDComponents/ImGuiTextEl.h"
#include "HUD/HUDComponents/ImGuiWindow.h"
#include "PissActor.h"

class ChessGameMode : public IGameMode
{
	struct ChessBoardState {
		bool teamBlack;
		PissActor::Type type;
	};
private:

	 std::map<std::pair<int, int>, ChessBoardState> currentChessBoardState;
	// Inherited via IGameMode
	 std::vector<std::pair<int,int>> getPossibleMove(PissActor& actor) {
		switch (actor.getId()) {
		//case PissActor::Type::ROOK:
		case PissActor::Type::PAWN:
			std::cout << "This is a pawm";
		//case PissActor::Type::BISHOP:
		//case PissActor::Type::KNIGHT:
		//case PissActor::Type::QUEEN:
		//case PissActor::Type::KING:
		default:
			std::cout << "This is empty";
		}
		return std::vector<std::pair<int, int>>();
	}
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

