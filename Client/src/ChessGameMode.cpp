#include "ChessGameMode.h"

void ChessGameMode::OnTick(float)
{
	//LOG_COLOR("Chess on tick", COLOR::BLUE, COLOR::BLACK);
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

ChessGameMode::ChessGameMode(IWorld& world)
	: IGameMode(world)
{
}
