#pragma once

#include <iostream>
#include <typeindex>
#include <map>
#include <memory>
#include <vector>

template <class Ty>
class Locator
{
private:
	std::map<std::type_index, std::unique_ptr<Ty>> services_;
public:
	template <class SubTy, class ... Args,
		std::enable_if_t<!std::is_array_v<SubTy>, int> = 0,
		std::enable_if_t<std::is_constructible_v<SubTy, Args&&...>, int> = 0,
		std::enable_if_t<std::is_base_of_v<Ty, SubTy>, int> = 0>
	inline void Add(Args&&... args)
	{
		if (services_[typeid(SubTy)])
		{
			std::cout << "Warning: Service '" << typeid(SubTy).name() << "' has already been added!" << std::endl;
			return;
		}

		std::unique_ptr<Ty> newService(new SubTy(std::forward<Args>(args)...));
		services_[typeid(SubTy)] = std::move(newService);
	};

	template <class SubTy, std::enable_if_t<std::is_base_of_v<Ty, SubTy>, int> = 0>
	inline SubTy& Get() const
	{
		if (services_.find(typeid(SubTy)) == services_.end())
		{
			//std::cout << "Fatal: Service '" << typeid(SubTy).name() << "' does not exist!" << std::endl;
			throw std::logic_error("Fatal: Service '" + std::string(typeid(SubTy).name()) + "' does not exist!");
		}

		return dynamic_cast<SubTy&>(*(services_.find(typeid(SubTy)))->second);
	};

	template <class SubTy, std::enable_if_t<std::is_base_of_v<Ty, SubTy>, int> = 0>
	inline void Remove()
	{
		if (!services_[typeid(SubTy)])
		{
			std::cout << "Warning: Service '" << typeid(SubTy).name() << "' has not been added before!" << std::endl;
			return;
		}
		services_.erase(typeid(SubTy));
	};

	~Locator() {
		services_.clear();
	};
};
