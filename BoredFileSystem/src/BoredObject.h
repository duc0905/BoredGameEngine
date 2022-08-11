#pragma once
#include<string>

class BoredObject
{
private:
	unsigned int id_;
	static unsigned int currentId_;

	void CreateId();
public:
	BoredObject();
	inline unsigned int GetID() { return id_; };
};

