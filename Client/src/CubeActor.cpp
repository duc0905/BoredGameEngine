#include "pch.h"
#include "CubeActor.h"
#include "Components/AudioComponent.h"

CubeActor::CubeActor():Actor()
{
	//LOG_COLOR("Begin CubeActor constractur", COLOR::BLUE, COLOR::BLACK);
	auto meshComp = CreateComponent<MeshComponent>();
	auto audioComp = CreateComponent<AudioComponent>();
	audioComp->SetSound("sounds/bounce.wav");
	meshComp->LoadMesh("3d_models/chessBoard1.obj");
	//LOG_COLOR("End CubeActor constractur", COLOR::BLUE, COLOR::BLACK);
}
