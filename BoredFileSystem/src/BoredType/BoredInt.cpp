#include "BoredInt.h"

std::string BoredInt::ToString()
{
	if (data == 0) {
		return "0";
	}
	int temp = abs(data);
	bool negative = data < 0;
	std::string ans = "";

	while (temp != 0)
	{
		int lastDigit = temp % 10;
		ans = (char)(lastDigit + '0') + ans;
		temp /= 10;
	}

	std::string anss = negative ? "-" + ans : ans;
	return ans;
}
