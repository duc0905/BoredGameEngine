#pragma once
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../System.h"
#include "../World/World.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderClass.h"

#include "Mesh.h"
#include "../Actor/Actor.h"
#include "../Components/MeshComponent.h"
#include "../Actor/BaseCamera.h"

#include "../Utils/Helper.h"

class Renderer :
    public System
{
private:
    //Shader defaultShader_;
    
    class QuadDrawer {
    private:
        std::unique_ptr<VertexArray> va_;
        std::unique_ptr<VertexBuffer> vb_;
        std::unique_ptr<IndexBuffer> ib_;
        Shader shader_;
    public:
        QuadDrawer() = default;
        void Init();
        void Draw(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
    };

    Shader meshShader_;
    QuadDrawer quadDrawer_;
public:
    Renderer() {};
    virtual ~Renderer() override
    {
        std::cout << "Cleaning up Renderer!" << std::endl;
    }

    void Render(World& world);
    void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color);

    void Draw(const Mesh& mesh);
    void Draw(const Actor& actor);

    // Inherited via System
    virtual void Init() override;

    void UseCamera(std::shared_ptr<BaseCamera> cam)
    {
        activeCam_ = cam;
    }
private:
    std::shared_ptr<BaseCamera> activeCam_;
};

