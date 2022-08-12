#include <iostream>
#include "BoredType/BoredType.h"
#include<filesystem>
#include<iostream>

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

    std::string path = "../Client/SavedBoredData";
    for (const auto& entry : std::filesystem::directory_iterator(path))
        std::cout << entry.path() << std::endl;

    std::cout << m.ToString();
}
