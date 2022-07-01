#pragma once
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../System.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderClass.h"

class Renderer :
    public System
{
private:
    //Shader defaultShader_;
    
    class QuadDrawer {
        std::unique_ptr<VertexArray> va_;
        std::unique_ptr<VertexBuffer> vb_;
        std::unique_ptr<IndexBuffer> ib_;
        Shader shader_;
    public:
        QuadDrawer() = default;
        void Init();
        void Draw(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
    };

    QuadDrawer quadDrawer_;
public:
    Renderer(IGame* game) : System(game) {};

    void Render();
    void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color);

    // Inherited via System
    virtual void Init() override;
};

