#pragma once
#include "pch.h"
#include "World/IWorld.h"
#include "Window/IWindow.h"
#include "Input/IInput.h"
#include "Renderer/IRenderer.h"
#include "Audio/IAudio.h"
#include "HUD/IHUD.h"

class IGame
{
private:
	static void Loop();
	static void Stop();

	static std::shared_ptr<IWorld> worldSystem_;
	static std::shared_ptr<IWindow> windowSystem_;
	static std::shared_ptr<IInput> inputSystem_;
	static std::shared_ptr<IRenderer> rendererSystem_;
	static std::shared_ptr<IAudio> audioSystem_;
	static std::shared_ptr<IHUD> hudSystem_;

	static std::map<std::type_index, std::shared_ptr<System>> systems_;

	// Not allowing anyone to create instance
	IGame() = default;
public:
	static void Run();

	static IWorld& GetWorld();
	//static std::shared_ptr<IWorld> GetWorldPtr() { return worldSystem_; };
	static void SetWorld(std::shared_ptr<IWorld> world);

	static IWindow& GetWindow();
	//static std::shared_ptr<IWindow> GetWindowPtr() { return windowSystem_; };
	static void SetWindow(std::shared_ptr<IWindow> window);

	static IInput& GetInput();
	//static std::shared_ptr<IInput> GetInputPtr() { return inputSystem_; };
	static void SetInput(std::shared_ptr<IInput> input);

	static IRenderer& GetRenderer();
	//static std::shared_ptr<IRenderer> GetRendererPtr() { return rendererSystem_; };
	static void SetRenderer(std::shared_ptr<IRenderer> renderer);

	static IAudio& GetAudio();
	//static std::shared_ptr<IAudio> GetAudioPtr() { return audioSystem_; };
	static void SetAudio(std::shared_ptr<IAudio> audio);

	static IHUD& GetHUD();
	//static std::shared_ptr<IHUD> GetHUDPtr() { return hudSystem_; };
	static void SetHUD(std::shared_ptr<IHUD>);

	template <class SubSys, class ...Args,
		std::enable_if_t<std::is_base_of_v<System, SubSys>, int> = 0,
		std::enable_if_t<std::is_constructible_v<SubSys, Args...>, int> = 0>
	static std::shared_ptr<SubSys> CreateCustomSystem(Args&&... args)
	{
		if (std::is_base_of_v<IWorld, SubSys>
			|| std::is_base_of_v<IWindow, SubSys>
			|| std::is_base_of_v<IInput, SubSys>
			|| std::is_base_of_v<IRenderer, SubSys>
			|| std::is_base_of_v<IAudio, SubSys>
			|| std::is_base_of_v<IHUD, SubSys>)
		{
			LOG_COLOR("You cannot create a ciritical system with this function. Use Set<YourSystem> instead (SetRenderer/SetInput).", COLOR::RED, COLOR::BLACK);
			return nullptr;
		}

		try {
			std::shared_ptr<System> s = systems_.at(typeid(SubSys));
		//	// Already an instance, override and Log wawrning to the console
			LOG_COLOR(std::string("You have already initiated another instance of ") 
				+ std::string(typeid(SubSys).name()) 
				+ std::string(". Overriding!"), COLOR::YELLOW, COLOR::BLACK);
		}
		catch (std::out_of_range) {}

		std::shared_ptr<SubSys> newSys = std::make_shared<SubSys>(args...);
		systems_[typeid(SubSys)] = newSys;
		return newSys;
	}
};

