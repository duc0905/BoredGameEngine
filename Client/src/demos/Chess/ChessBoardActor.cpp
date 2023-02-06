#include "pch.h"
#include "ChessBoardActor.h"
#include "Components/AudioComponent.h"

ChessBoardActor::ChessBoardActor():Actor()
{
	//LOG_COLOR("Begin ChessBoardActor constractur", COLOR::BLUE, COLOR::BLACK);
	auto meshComp = CreateComponent<MeshComponent>();
	auto audioComp = CreateComponent<AudioComponent>();
	auto transComp = FindComponent<TransformComponent>();
	transComp->Rotate({glm::pi<float>() / 2, 0, 0});
	audioComp->SetSound("sounds/bounce.wav");
	meshComp->LoadMesh("3d_models/chessBoard1.obj");
	//LOG_COLOR("End ChessBoardActor constractur", COLOR::BLUE, COLOR::BLACK);
}
