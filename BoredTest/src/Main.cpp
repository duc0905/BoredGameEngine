#include "TestSuite.h"
#include <map>

class MyTest : public TestSuite
{
public:
    virtual void Init() override
    {
        TestSuite::Init();
    }

    // Inherited via TestSuite
    virtual void Execute() override
    {
        TestInt();
    }
private:
    void TestInt()
    {
        ASSERT(1 + 1 == 1);
    }
};



int main()
{
    TEST(MyTest);
    return 0;
}