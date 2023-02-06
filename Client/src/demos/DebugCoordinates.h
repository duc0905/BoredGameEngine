#pragma once
#include "IGame.h"
#include "DemoUtils.h"

class DemoDebugSystem : public System
{
private:
	IRenderer& renderer;
	Context c;
	IRenderer::Line linex{ { -100,0,0 }, { 100,0,0 }, { 1,0,0,1 } },
			liney{ { 0,-100,0 }, { 0,100,0 }, { 0,1,0,1 } },
			linez{ { 0,0,-100 }, { 0,0,100 }, { 0,0,1,1 } };
public:
	DemoDebugSystem(): renderer(IGame::GetRenderer())
	{
		renderer.DrawLine(linex);
		renderer.DrawLine(liney);
		renderer.DrawLine(linez);

		auto& input = IGame::GetInput();
		input.AddContext(&c);
		c.AddActionMapping(KeyInput::KEY_0, 0, "HideDebug");
		input.BindAction("HideDebug", [&](KeyInput::Action action) -> void {
			if (action == KeyInput::PRESS)
			{
				linex.hide = true;
				liney.hide = true;
			}
		if (action == KeyInput::RELEASE)
		{
			linex.hide = false;
			liney.hide = false;
		}
			});
	}

	virtual void OnTick(float dt) override
	{
	}
};

void DebugCoordDemo()
{
	SYSTEM_ACCESS;
	
	IGame::CreateCustomSystem<DemoDebugSystem>();
}
