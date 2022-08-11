#pragma once
#include "BoredData.h"
#include<memory>
#include<vector>

class BoredArray: public BoredData
{
public:
	typedef std::vector<std::shared_ptr<BoredData>> arraysDataType;
private:
	std::vector<std::shared_ptr<BoredData>> data;
public:
	BoredArray() = default;
	BoredArray(const arraysDataType& arr);


	void PushBack(BoredData* elem);
	/*void Add();
	void Remove();*/
	
	arraysDataType::value_type& operator[](const int index);

	// Inherited via BoredData
	virtual std::string ToString() override;
};

