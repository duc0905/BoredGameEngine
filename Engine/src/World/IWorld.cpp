#include "pch.h"
#include "IWorld.h"

std::shared_ptr<IWorld> IWorld::defaultWorld_ = std::make_shared<NullWorld>();

bool IWorld::Start()
{
	if (!gm_)
	{
		LOG_COLOR("Cannot start: no GameMode is used!", COLOR::YELLOW, COLOR::BLACK);
		return false;
	}

	if (!(gm_->StartLevel()))
		return false;

	gm_->OnLevelStart();
	return true;
}

bool IWorld::Pause()
{
	if (!gm_)
	{
		LOG_COLOR("Cannot pause: no GameMode is used!", COLOR::YELLOW, COLOR::BLACK);
		return false;
	}

	if (!(gm_->PauseLevel()))
		return false;

	gm_->OnLevelPause();
	return true;
}

bool IWorld::Resume()
{
	if (!gm_)
	{
		LOG_COLOR("Cannot resume: no GameMode is used!", COLOR::YELLOW, COLOR::BLACK);
		return false;
	}

	if (!gm_->ResumeLevel())
		return false;

	gm_->OnLevelResume();
	return true;
}

bool IWorld::End()
{
	if (!gm_)
	{
		LOG_COLOR("Cannot end: no GameMode is used!", COLOR::YELLOW, COLOR::BLACK);
		return false;
	}

	if (!gm_->EndLevel())
		return false;

	gm_->OnLevelEnd();
	return true;
}
