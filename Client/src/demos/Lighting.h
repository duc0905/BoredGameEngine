#pragma once
#include "IGame.h"
#include "DemoUtils.h"
#include <Components/AmbientLightComponent.h>

void LightingDemo()
{
	SYSTEM_ACCESS

	std::shared_ptr<Actor> ambientLight = std::make_shared<Actor>();
	world.AddActor(ambientLight);
	auto ligit = ambientLight->CreateComponent<AmbientLightComponent>();
	ligit->color_ = { 1.f, 1.f, 1.f };
	ligit->strength_ = 0.7f;
}
