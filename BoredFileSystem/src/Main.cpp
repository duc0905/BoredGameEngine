#include <iostream>
#include "BoredInt.h"
#include "BoredMap.h"
#include "BoredFloat.h"
#include "BoredString.h"
#include "BoredArray.h"

int main()
{
    BoredMap m;
    m["abcd"] = new BoredInt(-5);

    BoredMap m2;
    m2["xxx"] = new BoredInt(10);

    m["xyz"] = &m2;
    m["Float"] = new BoredFloat(-43.52341f);
    m["String"] = new BoredString("Hello");
    BoredArray arr1;
    BoredArray arr2;
    arr1.PushBack(new BoredInt(-5));
    arr1.PushBack(new BoredString("Fuk"));
    m["Array"] = &arr1;
    m["EArray"] = &arr2;

    std::cout << m.ToString();
}
