#pragma once
#include "../pch.h"

#include "../Renderer/Mesh.h"
#include "Component.h"

class MeshComponent :
    public Component
{
private:
    std::shared_ptr<Mesh> mesh_;
public:
    // Inherited via Component
    virtual const std::string& GetComponentName() override;

    void LoadMesh(const std::string& filePath);

    void UseMesh(const std::shared_ptr<Mesh>& mesh);
    Mesh& GetMesh() const;
};

