#include "BoredMap.h"

BoredMap::BoredMap(const coreDataType& otherMap)
{
    data.clear();
    data = otherMap;
}

BoredMap::coreDataType::mapped_type& BoredMap::operator[](const std::string& key)
{
    return data[key];
}

std::string BoredMap::ToString()
{
    std::string ans = "{";

    coreDataType::iterator it = data.begin();
    ans += "\"" + it->first + "\": " + it->second->ToString();

    it++;
    while (it != data.end())
    {
        ans += ",";
        ans += "\"" + it->first + "\": " + it->second->ToString();
        it++;
    }

    ans += "}";

    return ans;
}
