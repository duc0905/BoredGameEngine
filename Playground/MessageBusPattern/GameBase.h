#pragma once
#include <iostream>

#include "World.h"
#include "Window.h"

class GameBase
{
private:
	int gameLogic_ = 0;
	Window window_;
	World world_;
	bool isRunning_ = true;
	bool isInit_ = false;

	void ProcessInput() {}
	void UpdateWorldState();
	void UpdateRenderState() {}
	void Render() {}
public:
	GameBase() {};
	GameBase(const Window&, const int&, const World&);
	~GameBase();

	virtual void OnInit() = 0;
	virtual void Loop();
	virtual void Stop();
	virtual void Start();
	virtual void OnDelete() = 0;
};

