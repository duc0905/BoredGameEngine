#pragma once
#include <glm/glm.hpp>
#include "Actor.h"

class BaseCamera : public Actor
{
public:
	virtual glm::mat4 GetViewMat() const = 0;
	virtual glm::mat4 GetProjectionMat() const = 0;
	virtual ~BaseCamera() {};
};

