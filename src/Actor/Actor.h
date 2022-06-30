#pragma once

#include <typeindex>
#include <map>
#include <iostream>

#include "Component.h"

class Actor
{
private:
	std::map<std::type_index, Component*> components_;
	Component* FindComponent(const std::type_index& ti);
public:
	Actor() = default;
	void AddComponent(Component* comp);

	template <class T>
	Component* FindComponent();
	Component* FindComponent(Component* comp);
};
