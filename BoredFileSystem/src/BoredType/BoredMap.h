#pragma once
#include <map>
#include "BoredData.h"

class BoredMap : public BoredData
{
public:
	typedef std::map<std::string, BoredData*> coreDataType;
private:
	coreDataType data;
public:
	BoredMap() = default;
	BoredMap(const coreDataType& otherMap);

	coreDataType::mapped_type& operator[](const std::string& key);

	// Inherited via BoredData
	virtual std::string ToString() override;
};

