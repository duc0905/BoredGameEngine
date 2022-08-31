#pragma once
#include "World/GameMode/IGameMode.h"

class ChessGameMode : public IGameMode
{
private:
	// Inherited via IGameMode
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

