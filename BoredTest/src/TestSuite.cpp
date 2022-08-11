#include "pch.h"
#include "TestSuite.h"

void TestSuite::Assert(bool& val, const char* func, const char* file, const int& line)
{
	if (!testInit_)
	{
		std::stringstream ss;
		ss << "ASSERT must be called within a callback within DESCRIBE." << std::endl;
		ss << "at file: " << file << ", line: " << line;

		LOG_COLOR(ss.str(), COLOR::YELLOW, COLOR::BLACK);
		return;
	}

	if (!val)
	{
		std::stringstream ss;
		ss << "Failed to assert in function " << "\"" << func << "\"" << std::endl;
		ss << "File: " << file << ", at line: " << line;
		LOG_COLOR(ss.str(), COLOR::RED, COLOR::BLACK);

		testPassed_ = false;
	}
}

void TestSuite::Assert(bool&& val, const char* func, const char* file, const int& line)
{
	Assert(val, func, file, line);
}

void TestSuite::PrepareTest(const std::string& desc)
{
	testPassed_ = true;
	testInit_ = true;
}

void TestSuite::CleanupTest(const std::string& desc)
{
	testInit_ = false;
	if (!testPassed_)
	{
		std::stringstream ss;
		ss << "Test case \"" << desc << "\"\tFAILED!";
		LOG_COLOR(ss.str(), COLOR::RED, COLOR::BLACK);
		numFailed_++;
	}
	else
	{
		std::stringstream ss;
		ss << "Test case \"" << desc << "\"\tpassed!";
		LOG_COLOR(ss.str(), COLOR::BRIGHT_GREEN, COLOR::BLACK);
		numPassed_++;
	}
}

TestSuite::~TestSuite()
{
}

std::string TestSuite::GetName()
{
	return name_;
}

void TestSuite::Init()
{
	if (isInit_) return;
	name_ = typeid(*this).name();
	isInit_ = true;
}

void TestSuite::Execute()
{
	if (isExecuted_) return;
	LOG_COLOR("Starting test suite: " + GetName(), COLOR::WHITE, COLOR::BLUE);
	isExecuted_ = true;
}

void TestSuite::Clear()
{
	if (isCleared_) return;

	COLOR text, bg;

	if (numFailed_)
	{
		bg = COLOR::YELLOW;
		text = COLOR::RED;
	}
	else
	{
		bg = COLOR::GREEN;
		text = COLOR::WHITE;
	}

	if (!numFailed_)
	{
		std::stringstream ss;
		ss << "Test suite " << GetName() << " passed | " << "Total: " << numPassed_ << " test cases.";
		LOG_COLOR(ss.str(), text, bg);
	}
	else
	{
		std::stringstream ss;
		ss << "Test suite " << GetName() << " FAILED | " << "Passed: " << numPassed_ << ", failed: " << numFailed_ << ".";
		LOG_COLOR("Test suite " + GetName() + " FAILED", text, bg);
	}

	isCleared_ = true;
}
