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
	bool isStarted_ = false;

	// This cannot be null
	IWorld& world_;

public:
	IGameMode(IWorld& world) : world_(world) {}

	bool IsPlaying() const { return isPlaying_; }
	bool IsEnded() const { return isEnded_; }
	bool IsStarted() const { return isStarted_; }

protected:
	virtual void OnLevelStart() = 0;
	virtual void OnLevelPause() = 0;
	virtual void OnLevelResume() = 0;
	virtual void OnLevelEnd() = 0;

private:
	virtual void OnAttach(IWorld& world) {};
	virtual void OnDetach() {};

	bool StartLevel() { 
		// TODO: Add log in this if statement
		if (isEnded_) 
		{
			LOG_COLOR("Cannot start: game is already ended!", COLOR::YELLOW, COLOR::BLACK);
			return false;
		}

		if (isStarted_)
		{
			LOG_COLOR("Cannot start: game is already started!", COLOR::YELLOW, COLOR::BLACK);
			return false;
		}

		LOG_COLOR("LEVEL STARTED!", COLOR::BLUE, COLOR::BLACK);
		isPlaying_ = true;
		isStarted_ = true;
		return true;
	};

	bool ResumeLevel() { 
		if (isPlaying_)
		{
			LOG_COLOR("Cannot resume: game is already being played!", COLOR::YELLOW, COLOR::BLACK);
			return false;
		}

		if (isEnded_)
		{
			LOG_COLOR("Cannot resume: game is already end!", COLOR::YELLOW, COLOR::BLACK);
			return false;
		}

		LOG_COLOR("LEVEL RESUMED!", COLOR::BLUE, COLOR::BLACK);
		isPlaying_ = true;
		return true;
	};
	bool PauseLevel() 
	{
		if (isEnded_)
		{
			LOG_COLOR("Cannot pause: game is already end!", COLOR::YELLOW, COLOR::BLACK);
			return false;
		}

		if (!isPlaying_)
		{
			LOG_COLOR("Cannot pause: game is already paused!", COLOR::YELLOW, COLOR::BLACK);
			return false;
		}

		LOG_COLOR("LEVEL PAUSED!", COLOR::BLUE, COLOR::BLACK);
		isPlaying_ = false; 
		return true;
	};
	bool EndLevel() 
	{ 
		if (isEnded_)
		{
			LOG_COLOR("Cannot end: game is already ended!", COLOR::YELLOW, COLOR::BLACK);
			return false;
		}

		LOG_COLOR("LEVEL ENDED!", COLOR::BLUE, COLOR::BLACK);
		isPlaying_ = false; isEnded_ = true;
		return true;
	};
};
