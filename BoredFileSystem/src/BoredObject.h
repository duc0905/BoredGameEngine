#pragma once
#include<string>
#include<iostream>
#include"BoredType/BoredType.h"

#define SETUP_CLASS(CLASSNAME)\
static std::string GetClassName() \
{ \
	return #CLASSNAME; \
} \

class BoredObject
{
private:
	unsigned int id_;
	static unsigned int currentId_;
	std::string name_;

	void CreateId();
public:
	BoredObject();
	virtual std::string ToString() = 0;
	inline BoredInt GetID() { return id_; };
};

