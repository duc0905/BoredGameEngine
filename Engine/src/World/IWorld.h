#pragma once
#include "../pch.h"
#include "../System.h"
#include "GameMode/IGameMode.h"
#include "../Actor/Actor.h"

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
	inline IGameMode& UseGameMode(Args&&... args)
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
				LOG_COLOR("Switching game mode to " + std::string(typeid(GM).name()), COLOR::GREEN, COLOR::BLACK);
				gm_->OnDetach();
				gm_.reset();
			}
		}

		gm_ = std::make_unique<GM>(args...);
		gm_->OnAttach(*this);
		return *gm_;
	}

	virtual void OnTick(float dt) override {}

	virtual bool Start() final;
	virtual bool Pause() final;
	virtual bool Resume() final;
	virtual bool End() final;

	virtual std::vector<std::shared_ptr<Actor>> GetActors() = 0;
	virtual Actor& GetActor(unsigned int getID) = 0;
	virtual void AddActor(std::shared_ptr<Actor> actor) = 0;
	virtual void RemoveActor(std::shared_ptr<Actor> actor) = 0;
};

class NullWorld : public IWorld
{
public:
	~NullWorld() {}
	// Inherited via IWorld
	virtual std::vector<std::shared_ptr<Actor>> GetActors() override { return std::vector<std::shared_ptr<Actor>>(); };
	virtual Actor& GetActor(unsigned int getID) override {
		return Actor::NullActor;
	}
	virtual void AddActor(std::shared_ptr<Actor> actor) override {};
	virtual void RemoveActor(std::shared_ptr<Actor> actor) override {};
};
