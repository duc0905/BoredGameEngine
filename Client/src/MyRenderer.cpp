#include "MyRenderer.h"

void MyRenderer::Init()
{
	lineShader_ = Shader("line.vert", "line.frag");

	va = VertexArray();
	//vb = VertexBuffer(nullptr, 0, GL_DYNAMIC_DRAW);
	//float pos[] = {
	//	0,0,0,1,1,1
	//};
	vb = VertexBuffer(nullptr, 14 * sizeof(float), GL_DYNAMIC_DRAW);

	vb.SetLayout({ { "Pos", Float3, GL_FALSE },
		{ "Color", Float4, GL_FALSE}
		} );
	va.AddVertexBuffer(vb);
}
