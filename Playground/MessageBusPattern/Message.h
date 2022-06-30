#pragma once

#include <string>
//#include "Module.h"

enum class MessageType {
	LOG,
	INPUT,
	USER_CREATED,
	MODULE_INPUT_UPDATE,
};

//class Module;
class Message
{
private:
	//Module* from_;
	MessageType type_;
	bool handled_ = false;
public:
	Message(const MessageType& type
		//, Module* from = nullptr
	) : type_(type)
		//, from_(from) 
	{}

	inline virtual const MessageType GetType() const final { return type_; }
	inline virtual const bool IsHandled() const final { return handled_; }
	//inline const Module* From() const { return from_; }

	inline void Handled() { handled_ = true; }
};