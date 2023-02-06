#pragma once
#include "Renderer\Renderer.h"

class MyRenderer : public Renderer
{
	GLuint VAO, VBO;
    VertexArray va;
    VertexBuffer vb;

    Shader lineShader_;
    virtual void Init() override;
};

