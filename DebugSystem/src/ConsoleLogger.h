#pragma once
#include <iostream>
#include <windows.h>

#define LOG(msg) ConsoleLogger::Log(msg)
#define LOG_COLOR(msg, textCol, bgCol) ConsoleLogger::Log(msg, textCol, bgCol)
#define COLOR ConsoleLogger::OutputColor

class ConsoleLogger
{
	//static unsigned int outputColorCode_;
	static unsigned int defaultColorCode_;
	static HANDLE hConsole;

	ConsoleLogger() {}
public:
	enum OutputColor
	{
		BLACK	= 0,
		BLUE	= 1,
		GREEN	= 2,
		CYAN	= 3,
		RED		= 4,
		MAGENTA	= 5,
		YELLOW	= 6,
		WHITE	= 7,
		BRIGHT_BLACK	= 8,
		BRIGHT_BLUE		= 9,
		BRIGHT_GREEN	= 10,
		BRIGHT_CYAN		= 11,
		BRIGHT_RED		= 12,
		BRIGHT_MAGENTA	= 13,
		BRIGHT_YELLOW	= 14,
		BRIGHT_WHITE	= 15,
		DEFAULT			= 16
	};

	static void SetDefaultOutputColor(const OutputColor& text, const OutputColor& bg);

	static unsigned int GetOutputColorCode(const OutputColor& text, const OutputColor& bg);

	template <typename T>
	inline static void Log(const T& msg, OutputColor text, OutputColor bg)
	{
		SetConsoleTextAttribute(hConsole, GetOutputColorCode(text, bg));
		std::cout << msg << std::endl;
		SetConsoleTextAttribute(hConsole, defaultColorCode_);
	}

	template <typename T>
	inline static void Log(const T& msg)
	{
		SetConsoleTextAttribute(hConsole, defaultColorCode_);
		std::cout << msg << std::endl;
	}
};