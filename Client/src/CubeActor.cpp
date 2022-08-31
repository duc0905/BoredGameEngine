#include "CubeActor.h"

CubeActor::CubeActor()
{
	//LOG_COLOR("Begin CubeActor constractur", COLOR::BLUE, COLOR::BLACK);
	auto meshComp = CreateComponent<MeshComponent>();
	meshComp->LoadMesh("3d_models/cube.obj");
	//LOG_COLOR("End CubeActor constractur", COLOR::BLUE, COLOR::BLACK);
}
