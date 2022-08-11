#include "TestActor.h"
#include "BoredType/BoredType.h"

std::string TestActor::ToString()
{
    BoredMap returnData;
    returnData["TestActor" + this->GetID()];
    return returnData.ToString();
}
