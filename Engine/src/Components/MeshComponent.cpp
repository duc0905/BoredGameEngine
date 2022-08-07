#include "MeshComponent.h"

const std::string& MeshComponent::GetComponentName()
{
    return "Mesh";
}

void MeshComponent::UseMesh(const std::shared_ptr<Mesh>& mesh)
{
    mesh_ = mesh;
}

Mesh& MeshComponent::GetMesh() const
{
    return dynamic_cast<Mesh&>(*mesh_);
}

/*
*  Create Cube actor V
*  Add Cube mesh to that actor V
*  Adjust transform component V
*  Write Draw function taking an Actor in renderer V
*  Create Camera Actor 
* 
*  Light actors
*  Compute light in shader
*/