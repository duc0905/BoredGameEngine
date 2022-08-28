#pragma once
#include "../../pch.h"
//#include "../World/IWorld.h"
#include "../../System.h"

class IWorld;

/**
* Base class of all GameMode. This is just an abstract class.
* Defines most of the logics for the game on the level scale
* Even if your game only has 1 level, still create a GameMode
* derived from this and attach to IGame.
*/
class IGameMode : public System
{
	friend class IWorld;
private:
	bool isPlaying_ = false;
	bool isEnded_ = false;

	// This cannot be null
	IWorld& world_;

public:
	IGameMode(IWorld& world) : world_(world) {}

	bool IsPlaying() const { return isPlaying_; }
	bool IsEnded() const { return isEnded_; }

protected:
	virtual void OnLevelStart() = 0;
	virtual void OnLevelPause() = 0;
	virtual void OnLevelResume() = 0;
	virtual void OnLevelEnd() = 0;

private:
	virtual void OnAttach(IWorld& world) {};
	virtual void OnDetach() {};

	void StartLevel() { 
		// TODO: Add log in this if statement
		if (isEnded_) return;
		isPlaying_ = true;
	};
	void ResumeLevel() { 
		// TODO: Add log in this if statement
		if (isEnded_) return;
		isPlaying_ = true; 
	};
	void PauseLevel() { isPlaying_ = false; };
	void EndLevel() { isPlaying_ = false; isEnded_ = true; };
};
