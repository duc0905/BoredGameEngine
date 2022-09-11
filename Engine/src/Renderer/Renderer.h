#pragma once
#include "../pch.h"

#include <GLFW/glfw3.h>
#include "IRenderer.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/IndexBuffer.h"
#include "Texture/OpenGLTexture.h"
#include "Buffers/FrameBuffer.h"
#include "ShaderClass.h"
#include "../Components/MeshComponent.h"

class Renderer : public IRenderer
{
private:
    Shader meshShader_;
    FrameBuffer* fbo;
    std::shared_ptr<OpenGLTexture> colorBuffer;
    std::shared_ptr<OpenGLTexture> idBuffer;
public:
    Renderer() {};
    virtual ~Renderer()
    {
        //std::cout << "Cleaning up Renderer!" << std::endl;
    }

    virtual void Render(IWorld& world) override;

    void Draw(const Mesh& mesh);
    virtual unsigned int GetMouseHoverEntityID(int x, int y) override;
    virtual void Draw(const Actor& actor) override;

    // Inherited via System
    virtual void Init() override;

    virtual void UseCamera(std::shared_ptr<BaseCamera> cam) override
    {
        activeCam_ = cam;
    }
private:
    std::shared_ptr<BaseCamera> activeCam_;
};

