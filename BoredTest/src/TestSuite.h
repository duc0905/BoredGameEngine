#include <typeindex>
#include <sstream>
#include <functional>
#include <vector>
#include "ConsoleLogger.h"

#define ASSERT(val) Assert(val, __func__, __FILE__, __LINE__)

#define DESCRIBE(desc, func) AddTest(desc, [&]() -> void {	\
	func();	\
})

#define TEST(testClass) Test<testClass>()

class TestSuite;

template <class T, std::enable_if_t<std::is_base_of_v<TestSuite, T>, int> = 0>
void Test()
{
	T* test = new T();
	test->TestSuite::Init();
	test->Init();

	test->TestSuite::Execute();
	test->Execute();

	test->TestSuite::Clear();
	test->Clear();
}

static unsigned int testCount = 0;

class TestSuite
{
private:
	unsigned int numPassed_ = 0;
	unsigned int numFailed_ = 0;


	std::string name_;
	bool isInit_ = false;
	bool isExecuted_ = false;
	bool isCleared_ = false;

	std::string GetName();
public:
	virtual void Init();
	virtual void Execute();
	virtual void Clear();

	virtual void AddTest(const std::string& name, std::function<void()> func) final
	{
		PrepareTest(name);	
		BeforeEach();	
		func();
		AfterEach();	
		CleanupTest(name);	
	}
	~TestSuite();
protected:
	void Assert(bool& val, const char* func, const char* file, const int& line);
	void Assert(bool&& val, const char* func, const char* file, const int& line);

	virtual void PrepareTest(const std::string& desc) final;
	virtual void CleanupTest(const std::string& desc) final;

	virtual void BeforeEach() {}
	virtual void AfterEach() {}

private:
	bool testInit_ = false;
	bool testPassed_ = false;

};