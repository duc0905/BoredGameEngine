#pragma once
#include <typeindex>
#include <map>
#include <iostream>

#include "../BoredObject.h"
#include "../Component/IComponent.h"

class IActor :
    public BoredObject
{
private:
	std::map<std::type_index, std::shared_ptr<IComponent>> components_;
public:
	IActor();

	// Called when added to World
	virtual void OnAttach() {};

	virtual void OnDetach() {};

	virtual void OnUpdate(float dt) {};

	template <class SubCom, class ...Args,
		std::enable_if_t<std::is_base_of_v<IComponent, SubCom>, int> = 0,
		std::enable_if_t<std::is_constructible_v<SubCom, Args...>, int> = 0>
	inline std::shared_ptr<SubCom> CreateComponent(Args&&... args)
	{
		try {
			auto comp = FindComponent<SubCom>();
			return comp;
		}
		catch (std::exception&)
		{
			std::shared_ptr<SubCom> newComp = std::make_shared<SubCom>(args...);
			components_[typeid(SubCom)] = newComp;
			return newComp;
		}
	}

	template <class SubCom, std::enable_if_t<std::is_base_of_v<IComponent, SubCom>, int> = 0>
	std::shared_ptr<SubCom> FindComponent() const
	{
		auto it = components_.find(typeid(SubCom));
		if (it == components_.end())
		{
			throw std::exception(
				(std::string("Actor does not have component of type: ") + std::string(typeid(SubCom).name())).c_str()
			);
		}

		return std::dynamic_pointer_cast<SubCom>(it->second);
	}

	virtual BoredMap Serialize() = 0;
};

