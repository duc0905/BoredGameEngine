#include "Actor.h"

void Actor::AddComponent(Component* comp)
{
	components_[typeid(comp)] = comp;
}

template<class T>
Component* Actor::FindComponent()
{
	if (!std::is_base_of_v<Component, T>)
	{
		std::cout << "Warning: '" << typeid(T).name << "' is not derived from Component" << std::endl;
		return nullptr;
	}

	return FindComponent(typeid(T));
}

Component* Actor::FindComponent(Component* comp)
{
	if (comp == nullptr) return nullptr;

	return FindComponent(typeid(comp));
}

Component* Actor::FindComponent(const std::type_index& ti)
{
	for (auto it{ components_.begin() }; it != components_.end(); it++)
		if (it->first == ti)
			return it->second;

	return nullptr;
}