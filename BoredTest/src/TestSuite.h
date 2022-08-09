#include <typeindex>
#include <sstream>
#include "ConsoleLogger.h"

#define ASSERT(val) Assert(val, __func__, __FILE__, __LINE__)

static unsigned int testCount = 0;

class TestSuite
{
private:
	std::string name_;
	bool isInit_ = false;

	std::string GetName();
public:
	virtual void Init();
	virtual void Execute() = 0;
	virtual void Clear() {};
protected:
	//void Assert(bool val, const std::string& func, const std::string& file, const std::string& line);
	void Assert(bool& val, const char* func, const char* file, const int& line);
	void Assert(bool&& val, const char* func, const char* file, const int& line);
};