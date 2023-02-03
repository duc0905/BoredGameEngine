#pragma once
#include "World/GameMode/IGameMode.h"
#include "PissActor.h"

class ChessGameMode : public IGameMode
{
	struct ChessBoardState {
		std::pair<int, int> pos;
		bool team;
		PissActor::Type type;
	};
private:

	 std::vector<ChessBoardState> currentChessBoardState;
	// Inherited via IGameMode
	std::vector<std::pair<int,int>> getPossibleMove(PissActor& actor) {
		/*switch (actor.getId()) {
		case PissActor::Type::ROOK:
		case PissActor::Type::PAWN:
		case PissActor::Type::BISHOP:
		case PissActor::Type::KNIGHT:
		case PissActor::Type::QUEEN:
		case PissActor::Type::KING:
		default:
		}*/
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
	ChessGameMode(IWorld& world);
};

