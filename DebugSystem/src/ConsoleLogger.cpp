#include "pch.h"
#include "ConsoleLogger.h"

//unsigned int ConsoleLogger::outputColorCode_ = 15;
unsigned int ConsoleLogger::defaultColorCode_ = 15;
HANDLE ConsoleLogger::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void ConsoleLogger::SetDefaultOutputColor(const OutputColor& text, const OutputColor& bg)
{
	defaultColorCode_ = GetOutputColorCode(text, bg);
}

unsigned int ConsoleLogger::GetOutputColorCode(const OutputColor& text, const OutputColor& bg)
{
	return (bg % 16) * 16 + text % 16;
}