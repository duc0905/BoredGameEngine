#include "pch.h"
#include "Pawn.h"

std::shared_ptr<Mesh> Pawn::mesh_ = std::make_shared<Mesh>();

Pawn::Pawn(int x, int y, int team):PissActor(x,y,team)
{
	//LOG_COLOR("Begin ChessBoardActor constractur", COLOR::BLUE, COLOR::BLACK);
	auto meshComp = CreateComponent<MeshComponent>();
	if (!mesh_->IsInit()) {
		mesh_->LoadModel("3d_models/pawn.gltf");
	}
	meshComp->UseMesh(mesh_);

	auto transComp = FindComponent<TransformComponent>();
	transComp->Scale({ 0.3f, 0.3f, 0.3f });
	transComp->Translate({ -1.2f, 0.0f, 0.0f });
	//LOG_COLOR("End ChessBoardActor constractur", COLOR::BLUE, COLOR::BLACK);
}
