#pragma once

class System
{
public:
	System() {};

	virtual void OnTick(float dt) {}
	virtual void Init() {};
	virtual ~System() {}
};