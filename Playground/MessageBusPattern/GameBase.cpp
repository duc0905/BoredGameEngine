#include "GameBase.h"

void GameBase::UpdateWorldState()
{
	// UpdatePhysics(); // How water flows or a bullet flies
	// CheckForCollisions(); // Find what objects collided this frame
	// ExecutePlayerActions(); // Map player input to gameplay actions
	// UpdateEnvironment();
	// UpdateAI();
}

GameBase::GameBase(const Window& myWindow, const int& myGameLogic, const World& myWorld)
	: window_(myWindow), gameLogic_(myGameLogic), world_(myWorld), isInit_(true)
{
}


void GameBase::Start()
{
	OnInit();
	Loop();
	Stop();
}

void GameBase::OnInit()
{
	// Initialize fundamental system

	// Logging

	// Rendering

	// Audio

	// Memory
}

void GameBase::Loop()
{
	while (isRunning_) {
		// Process input
		ProcessInput();
		// Update world state
		UpdateWorldState();
		// Update render state
		UpdateRenderState();
		// Render
		Render();
	}
}

void GameBase::Stop()
{
	// Flush and stop fundamental systems

}

GameBase::~GameBase()
{
	Stop();
}
