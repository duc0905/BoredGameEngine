#pragma once
#include "pch.h"
#include "World/IWorld.h"
#include "Window/IWindow.h"
#include "Input/IInput.h"
#include "Renderer/IRenderer.h"

class IGame
{
private:
	static void Loop();
	static void Stop();

	static std::shared_ptr<IWorld> worldSystem_;
	static std::shared_ptr<IWindow> windowSystem_;
	static std::shared_ptr<IInput> inputSystem_;
	static std::shared_ptr<IRenderer> rendererSystem_;

	// Not allowing anyone to create instance
	IGame() = default;
public:
	static void Run();

	static IWorld& GetWorld();
	static std::shared_ptr<IWorld> GetWorldPtr() { return worldSystem_; };
	static void SetWorld(std::shared_ptr<IWorld> world);

	static IWindow& GetWindow();
	static std::shared_ptr<IWindow> GetWindowPtr() { return windowSystem_; };
	static void SetWindow(std::shared_ptr<IWindow> window);

	static IInput& GetInput();
	static std::shared_ptr<IInput> GetInputPtr() { return inputSystem_; };
	static void SetInput(std::shared_ptr<IInput> input);

	static IRenderer& GetRenderer();
	static std::shared_ptr<IRenderer> GetRendererPtr() { return rendererSystem_; };
	static void SetRenderer(std::shared_ptr<IRenderer> renderer);
};

