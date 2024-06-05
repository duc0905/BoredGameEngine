#pragma once

#include <memory>
#include "../Actor.hpp"

struct IDToPtr
{
	std::shared_ptr<Bored::Actor> ptr;
	IDToPtr() = default;
	IDToPtr(const IDToPtr& o) { ptr = o.ptr; }
	IDToPtr(std::shared_ptr<Bored::Actor> p) : ptr(p) {}
	operator std::shared_ptr<Bored::Actor>() { return ptr; }
};
