#include "CubeActor.h"

CubeActor::CubeActor()
{
	auto meshComp = CreateComponent<MeshComponent>();
	meshComp->LoadMesh("3d_models/cube.obj");
}
