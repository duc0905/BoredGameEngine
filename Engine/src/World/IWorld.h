#pragma once
#include "../pch.h"
#include "../Actor/Actor.h"
#include "../System.h"

class IWorld : public System
{
private:
	static std::shared_ptr<IWorld> defaultWorld_;
public:
	static std::shared_ptr<IWorld> GetDefault() { return defaultWorld_; }

	virtual ~IWorld() {}
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
