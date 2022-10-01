#pragma once
#include "LightComponent.h"
struct DiffuseLightComponent :
    public LightComponent
{
    // Inherited via LightComponent
    virtual const std::string& GetComponentName() override {
        return "DiffuseLightComponent";
    };
};

