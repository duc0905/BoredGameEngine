#include "BoredArray.h"
#include "BoredData.h"

BoredArray::BoredArray(const arraysDataType& arr)
{
	data.clear();
	data = arr;
}

void BoredArray::PushBack(BoredData* elem)
{
	data.push_back(std::shared_ptr<BoredData>(elem));
}

BoredArray::arraysDataType::value_type& BoredArray::operator[](const int index) {
	return data.at(index);
}

std::string BoredArray::ToString()
{
	std::string ans = "[";
	int size = data.size();
	if (size == 0) {
		ans += "]";
		return ans;
	}
	for (auto el : data) {
		ans += el->ToString();
		ans += ",";
	}
    ans.pop_back();
	ans += "]";
	return ans;
}

