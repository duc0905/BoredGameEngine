#pragma once
#include <iostream>

class IGame;
class System
{
private:
	IGame* Game_;
public:
	System(IGame* game) : Game_(game) {}
	inline const IGame& GetGame() const { return *Game_; }

	virtual void Init() = 0;
	virtual ~System() {}
};