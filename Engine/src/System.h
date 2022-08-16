#pragma once

class System
{
private:
	//IGame* Game_;
public:
	//System(IGame* game) : Game_(game) {}
	//inline const IGame& GetGame() const { return *Game_; }
	System() {};

	virtual void OnTick(float dt) {}
	virtual void Init() {};
	virtual ~System() {}
};