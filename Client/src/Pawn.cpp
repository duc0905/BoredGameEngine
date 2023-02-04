#include "pch.h"
#include "Pawn.h"

Pawn::Pawn():PissActor()
{
	//LOG_COLOR("Begin ChessBoardActor constractur", COLOR::BLUE, COLOR::BLACK);
	auto meshComp = CreateComponent<MeshComponent>();
	meshComp->LoadMesh("3d_models/pawnCube.obj");
	auto transComp = FindComponent<TransformComponent>();
	transComp->Scale({ 0.33f, 0.33f, 0.33f });
	transComp->Translate({ -1.2f, 0.0f, 0.0f });
	//LOG_COLOR("End ChessBoardActor constractur", COLOR::BLUE, COLOR::BLACK);
}

PissActor::Type Pawn::getId()
{
	return PissActor::Type::PAWN;
}

std::vector<std::pair<int, int>> Pawn::getPossileMoves()
{
	return std::vector<std::pair<int, int>>();
}
