#include "LogModule.h"

void LogModule::OnMessage(Message& msg)
{
	switch (msg.GetType()) {
	case MessageType::LOG:
		LogMessage& m = static_cast<LogMessage&>(msg);

		std::cout << "[";
		switch (m.GetSeverity()) {
		case LogSeverity::CASUAL:
			std::cout << "Casual";
			break;
		case LogSeverity::WARNING:
			std::cout << "Warning";
			break;
		case LogSeverity::CRITICAL:
			std::cout << "Critical";
			break;
		default:
			std::cout << "None";
			break;
		}
		std::cout << "]: " << m.GetText() << std::endl;
		break;
	}
}
