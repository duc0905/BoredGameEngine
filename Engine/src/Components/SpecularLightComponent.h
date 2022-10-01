#pragma once
#include "LightComponent.h"
struct SpecularLightComponent :
    public LightComponent
{
    // Inherited via LightComponent
    virtual const std::string& GetComponentName() override {
        return "SpecularLightComponent";
    };
};

