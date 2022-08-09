#include "TestSuite.h"

void TestSuite::Assert(bool& val, const char* func, const char* file, const int& line)
{
	if (!val)
	{
		std::stringstream ss;
		ss << "Failed to assert in function " << "\"" << func << "\"" << " | File: " << file << ", at line: " << line << std::endl;
		ConsoleLogger::Log(ss.str(), ConsoleLogger::RED, ConsoleLogger::BLACK);
	}
}

void TestSuite::Assert(bool&& val, const char* func, const char* file, const int& line)
{
	Assert(val, func, file, line);
}

std::string TestSuite::GetName()
{
	return name_;
}

void TestSuite::Init()
{
	name_ = typeid(*this).name();
	ConsoleLogger::Log(name_, ConsoleLogger::BLUE, ConsoleLogger::WHITE);
	isInit_ = true;
}
