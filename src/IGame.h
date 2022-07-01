#pragma once
#include <memory>
#include <vector>
#include "Utils/Locator.h"

#include "World/World.h"
#include "Window/Window.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"

class IGame : public Locator<System>
{
private:
	//std::vector<std::unique_ptr<Level>> Levels_;
	/*std::unique_ptr<World> World_;
	std::unique_ptr<Window> Window_;
	std::unique_ptr<Input> Input_;
	std::unique_ptr<Renderer> Renderer_;*/
	virtual void Init() = 0;
	void Loop();
	virtual void Stop() = 0;
public:
	void Run();
	virtual ~IGame();

	template <class SubSys, class ... Args>
	inline void AddSystem(Args&& ... args)
	{
		Add<SubSys>(args...);
		Get<SubSys>().Init();
	}
	/*template <class Sys, class ...Args>
	void CreateSystem(Args&&...);*/

	/*inline const World& GetWorld() { return *World_.get(); };
	void CreateWorld();
	inline void DeleteWorld() { World_.reset(); }

	void CreateWindow(const unsigned int& width, const unsigned int& height, const std::string& title = "My game");
	inline const Window& GetWindow() { return *Window_.get(); };
	inline void DeleteWindow() { Window_.reset(); };

	void InitInput();
	inline const Input& GetInput() { return *Input_.get(); }
	inline void DeleteInput() { Input_.reset(); }

	void InitRenderer();
	inline const Renderer& GetRenderer() { return *Renderer_.get(); }
	inline void DeleteRenderer() { Renderer_.reset(); }*/
};

