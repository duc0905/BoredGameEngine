#pragma once
#include <string>
#include "Message.h"

enum class LogSeverity {
	CRITICAL,
	WARNING,
	CASUAL,
	NONE
};

class LogMessage : public Message
{
private:
	std::string text_;
	LogSeverity severity_;
public:
	LogMessage(const std::string& text, const LogSeverity& severity)
		: Message(MessageType::LOG), text_(text), severity_(severity) {}
	inline const std::string GetText() const { return text_; }
	inline const LogSeverity GetSeverity() const { return severity_; }
};

