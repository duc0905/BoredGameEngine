#include "TestActor.h"
#include "BoredType/BoredType.h"

std::string TestActor::ToString()
{
    BoredMap returnData;
    returnData[TestActor::GetClassName()] = new BoredString("testing");
    returnData["AA"] = new BoredInt(-10);
    BoredArray arr1;
    BoredArray arr2;
    arr1.PushBack(new BoredInt(-5));
    arr1.PushBack(new BoredString("Fuk"));
    returnData["Array"] = &arr1;
    returnData["EArray"] = &arr2;
    BoredMap m2;
    m2["xxx"] = new BoredInt(10);
    returnData["xyz"] = &m2;
    returnData["Float"] = new BoredFloat(-43.52341f);
    returnData["String"] = new BoredString("Hello");
    return returnData.ToString();
}
