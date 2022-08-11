#include "BoredFloat.h"
#include<sstream> 
#include<string>

std::string BoredFloat::ToString()
{   
    /*float temp = abs(data);
    bool negative = data < 0.0f;
    std::string ans = "";
    int temp1 = temp;
    temp -= temp1;
    while (temp1 > 0) {
        int lastDigit = temp1 % 10;
        ans = (char)(lastDigit + '0') + ans;
        temp1 -= lastDigit;
        temp1 /= 10;
    }
    ans += ".";
    while (temp != 0) {
        int lastDigit = temp * 10;
        ans = ans + (char)(lastDigit + '0');
        temp *= 10;
        temp -= lastDigit;
    }

    return negative ? "-" + ans : ans;*/

    float temp = abs(data);
    bool negative = data < 0.0f;
    std::stringstream ss1;
    ss1 << temp;
    std::string ans = ss1.str();
    return negative ? "-" + ans : ans;
}
