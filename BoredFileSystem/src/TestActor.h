#pragma once
#include<string>
#include"BoredMap.h"
#include "BoredObject.h"
class TestActor :
    public BoredObject
{
    // Inherited via BoredObject
    virtual std::string ToString() override;
};

