#pragma once
#include <iostream>

class System
{
private:
	//IGame* Game_;
public:
	//System(IGame* game) : Game_(game) {}
	//inline const IGame& GetGame() const { return *Game_; }
	System() {};

	virtual void Init() = 0;
	virtual ~System() {}
};