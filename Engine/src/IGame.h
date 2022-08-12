#pragma once
#include <memory>
#include <vector>

#include "World/World.h"
#include "Window/Window.h"
#include "Input/Input.h"

#include "Renderer/IRenderer.h"

class IGame
{
private:
	static void Loop();
	static void Stop();

	static std::shared_ptr<World> worldSystem_;
	static std::shared_ptr<Window> windowSystem_;
	static std::shared_ptr<Input> inputSystem_;
	static IRenderer& rendererSystem_;
public:
	static void Run();
	virtual ~IGame() = delete;

	static World& GetWorld();
	static void SetWorld(World* world);

	static Window& GetWindow();
	static void SetWindow(Window* window);

	static Input& GetInput();
	static void SetInput(Input* input);

	static IRenderer& GetRenderer();
	static void SetRenderer(IRenderer* renderer);
};

