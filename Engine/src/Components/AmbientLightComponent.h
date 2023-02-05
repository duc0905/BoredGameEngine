#pragma once
#include "LightComponent.h"

struct AmbientLightComponent : public LightComponent
{

	// Inherited via LightComponent
    virtual const std::string& GetComponentName() override {
        return "AmbientLightComponent";
    };
};

