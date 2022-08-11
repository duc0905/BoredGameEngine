#include "BoredObject.h"

unsigned int BoredObject::currentId_ = 0;

void BoredObject::CreateId()
{
	id_ = currentId_;
	currentId_++;
}

BoredObject::BoredObject()
{
	CreateId();
}
