#pragma once
#include "BoredObject.h"
#include<string>
#include<memory>

class BoredFileSystem
{
public:
	std::string saveBject(std::shared_ptr<BoredObject> bject);
	std::string readBoredFile(const std::string& filename);
};

