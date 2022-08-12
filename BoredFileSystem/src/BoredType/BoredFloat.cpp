#include "BoredFloat.h"
#include<sstream> 
#include<string>

std::string BoredFloat::ToString()
{   
    float temp = abs(data);
    bool negative = data < 0.0f;
    std::stringstream ss1;
    ss1 << temp;
    std::string ans = ss1.str();
    return negative ? "-" + ans : ans;
}
