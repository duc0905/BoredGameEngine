#pragma once
#include<string>
#include "BoredObject.h"
class TestActor :
    public BoredObject
{
    SETUP_CLASS(TestActor);
    // Inherited via BoredObject
    virtual std::string ToString() override;
};

