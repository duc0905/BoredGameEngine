
#include "../pch.h"
#include "MeshComponent.h"

const std::string& MeshComponent::GetComponentName()
{
    return "Mesh";
}

void MeshComponent::LoadMesh(const std::string& filePath)
{
    mesh_ = std::make_shared<Mesh>();
    mesh_->LoadModel(filePath);
}

void MeshComponent::UseMesh(const std::shared_ptr<Mesh>& mesh)
{
    mesh_ = mesh;
}

Mesh& MeshComponent::GetMesh() const
{
    return *mesh_;
}