#pragma once
#include "../pch.h"

template <class Ty>
class Locator
{
private:
	std::map<std::type_index, std::shared_ptr<Ty>> services_;
public:
	template <class SubTy, class ... Args,
		std::enable_if_t<!std::is_array_v<SubTy>, int> = 0,
		std::enable_if_t<std::is_constructible_v<SubTy, Args&&...>, int> = 0,
		std::enable_if_t<std::is_base_of_v<Ty, SubTy>, int> = 0>
	inline std::shared_ptr<SubTy> Add(Args&&... args)
	{
		if (services_[typeid(SubTy)])
		{
			//std::cout << "Warning: Service '" << typeid(SubTy).name() << "' has already been added!" << std::endl;
			return std::dynamic_pointer_cast<SubTy>(services_[typeid(SubTy)]);
		}

		std::shared_ptr<SubTy> newService = std::make_shared<SubTy>(std::forward<Args>(args)...);
		services_[typeid(SubTy)] = newService;
		return newService;
	};

	template <class SubTy, std::enable_if_t<std::is_base_of_v<Ty, SubTy>, int> = 0>
	inline std::shared_ptr<SubTy> Get() const
	{
		auto it = services_.find(typeid(SubTy));
		if (it == services_.end())
		{
			throw std::logic_error("Fatal: Service '" + std::string(typeid(SubTy).name()) + "' does not exist!");
		}

		return std::dynamic_pointer_cast<SubTy>(it->second);
	};

	template <class SubTy, std::enable_if_t<std::is_base_of_v<Ty, SubTy>, int> = 0>
	inline void Remove()
	{
		if (!services_[typeid(SubTy)])
		{
			//std::cout << "Warning: Service '" << typeid(SubTy).name() << "' has not been added before!" << std::endl;
			return;
		}
		services_.erase(typeid(SubTy));
	};

	~Locator() {
		services_.clear();
	};
};
