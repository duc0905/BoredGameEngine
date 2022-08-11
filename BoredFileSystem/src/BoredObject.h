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
	virtual std::string ToString() = 0;
	inline unsigned int GetID() { return id_; };
};

