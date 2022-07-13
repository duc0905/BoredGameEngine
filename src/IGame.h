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
	virtual void Init() = 0;
	void Loop();
	virtual void Stop() = 0;
public:
	void Run();
	virtual ~IGame();

	template <class SubSys, class ... Args>
	inline std::shared_ptr<SubSys> AddSystem(Args&& ... args)
	{
		std::shared_ptr<SubSys> ss = Add<SubSys>(args...);
		ss->Init();
		return ss;
	}
};

