#pragma once
#include "../pch.h"

#include <GLFW/glfw3.h>
#include "IRenderer.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/IndexBuffer.h"
#include "Texture/OpenGLTexture.h"
#include "ShaderClass.h"
#include "../Components/MeshComponent.h"
#include "../Components/AmbientLightComponent.h"
#include "../Components/DiffuseLightComponent.h"
#include "../Components/SpecularLightComponent.h"

class Renderer : public IRenderer
{
private:

    Shader meshShader_;
    Shader screenShader_;
    Shader lineShader_;

    VertexArray* screenVao;
    VertexBuffer* screenVbo;
    IndexBuffer* screenIbo;

    FrameBuffer* fbo;
    std::shared_ptr<OpenGLTexture> colorBuffer;
    std::shared_ptr<OpenGLTexture> idBuffer;

    VertexArray* lineVao;
    VertexBuffer* lineVbo;
    std::vector<Line*> lines_;
public:
    Renderer()
        : fbo(nullptr), screenVao(nullptr), screenVbo(nullptr), screenIbo(nullptr), 
        lineVao(nullptr), lineVbo(nullptr)  {};
    virtual ~Renderer()
    {
        //std::cout << "Cleaning up Renderer!" << std::endl;
        delete fbo;
        delete screenVao;
        delete screenVbo;
        delete screenIbo;

        delete lineVao;
        delete lineVbo;
    }

    virtual void Render(IWorld& world) override;

    void Draw(const Mesh& mesh);
    virtual unsigned int GetMouseHoverEntityID(int x, int y) override;
    virtual void Draw(const Actor& actor) override;
    void Draw(const Line& line);
    virtual void DrawLine(Line& line) override;
    virtual void DrawLine(const Line& line) override;

    // Inherited via System
    virtual void Init() override;

    virtual void UseCamera(std::shared_ptr<BaseCamera> cam) override
    {
        activeCam_ = cam;
    }
private:
    std::shared_ptr<BaseCamera> activeCam_;
};

