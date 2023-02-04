#include "TileActor.h"

std::shared_ptr<Mesh> TileActor::whiteMesh_ = std::make_shared<Mesh>();
std::shared_ptr<Mesh> TileActor::blackMesh_ = std::make_shared<Mesh>();

TileActor::TileActor(bool isBlack)
{
	auto meshComp = CreateComponent<MeshComponent>();
	if (!whiteMesh_->IsInit()) {
		whiteMesh_->LoadModel("3d_models/chessBoardWhite.obj");
	}
	if (!blackMesh_->IsInit()) {
		blackMesh_->LoadModel("3d_models/chessBoardBlack.obj");
	}
	if (isBlack) {
		meshComp->UseMesh(blackMesh_);
	}
	else {
		meshComp->UseMesh(whiteMesh_);
	}
	auto transComp = FindComponent<TransformComponent>();
	transComp->Scale({ 0.5f, 0.5f, 0.5f });
	transComp->Rotate({ glm::pi<float>() / 2, 0, 0 });
}
