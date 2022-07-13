#pragma once
#include <memory>

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

    void UseMesh(const std::shared_ptr<Mesh>& mesh);
    Mesh& GetMesh() const;
};

