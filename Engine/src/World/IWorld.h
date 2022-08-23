#pragma once
#include "../pch.h"
#include "../GameMode/IGameMode.h"
#include "../Actor/Actor.h"
#include "../System.h"

class IWorld : public System
{
	friend class IGameMode;
private:
	static std::shared_ptr<IWorld> defaultWorld_;
protected:
	std::unique_ptr<IGameMode> gm_;
public:
	static std::shared_ptr<IWorld> GetDefault() { return defaultWorld_; }

	virtual ~IWorld() 
	{
		if (gm_)
		{
			gm_->OnDetach();
			gm_.reset();
		}
	}

	template <class GM, class ...Args,
		std::enable_if_t<std::is_base_of_v<IGameMode, GM>, int> = 0,
		std::enable_if_t<std::is_constructible_v<GM, Args...>, int> = 0>
	GM& UseGameMode(Args&&... args)
	{
		if (gm_)
		{
			if (gm_->IsPlaying())
			{
				LOG_COLOR("The game is playing, cannot use another game mode. Using the previous game mode.", COLOR::YELLOW, COLOR::BLACK);
				return *gm_;
			}
			else
			{
				LOG_COLOR("Switching game mode!", COLOR::GREEN, COLOR::BLACK);
				gm_->OnDetach();
				gm_.reset();
			}
		}

		gm_ = std::make_unique<GM>(args...);
		gm_->OnAttach(*this);
		return *gm_;
	}

	virtual void OnTick(float dt) override
	{
		// Tick the gamemode
		if (gm_)
			gm_->OnTick(dt);

		// Tick the actors
		for (auto actor : GetActors())
			actor->OnTick(dt);
	}

	virtual std::vector<std::shared_ptr<Actor>> GetActors() = 0;
	virtual void AddActor(std::shared_ptr<Actor> actor) = 0;
	virtual void RemoveActor(std::shared_ptr<Actor> actor) = 0;
};

class NullWorld : public IWorld
{
	// Inherited via IWorld
	virtual std::vector<std::shared_ptr<Actor>> GetActors() override { return std::vector<std::shared_ptr<Actor>>(); };
	virtual void AddActor(std::shared_ptr<Actor> actor) override {};
	virtual void RemoveActor(std::shared_ptr<Actor> actor) override {};
};
