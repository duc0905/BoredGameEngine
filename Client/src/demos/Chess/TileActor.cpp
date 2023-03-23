#include "TileActor.h"

std::shared_ptr<Mesh> TileActor::whiteMesh_ = std::make_shared<Mesh>();
std::shared_ptr<Mesh> TileActor::blackMesh_ = std::make_shared<Mesh>();

TileActor::TileActor(bool isBlack)
{
	auto meshComp = CreateComponent<MeshComponent>();
	if (!whiteMesh_->IsInit()) {
		whiteMesh_->LoadModel("3d_models/whiteTile.gltf");
	}
	if (!blackMesh_->IsInit()) {
		blackMesh_->LoadModel("3d_models/blackTile.gltf");
	}
	if (isBlack) {
		meshComp->UseMesh(blackMesh_);
	}
	else {
		meshComp->UseMesh(whiteMesh_);
	}
	auto transComp = FindComponent<TransformComponent>();
	transComp->Scale({ 0.5f, 0.5f, 0.5f });
	transComp->Rotate({ 0, glm::pi<float>() / 2, 0 });
}
