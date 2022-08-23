#include "MyRenderer.h"

void MyRenderer::Render(IWorld& world)
{
	//LOG_COLOR("IN RENDER", COLOR::GREEN, COLOR::BLACK);
	Renderer::Render(world);
}
