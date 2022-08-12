#include "TestSuite.h"

class MyTest : public TestSuite
{
public:
    virtual void Init() override
    {
    }

    // Inherited via TestSuite
    virtual void Execute() override
    {
        DESCRIBE("1 + 1 should be 2", TestInt);
        DESCRIBE("1.0 + 1.0 should be 2.0", TestFloat);
        DESCRIBE("Intentional fail", TestFail);
    }

private:
    void TestInt()
    {
        ASSERT(1 + 1 == 2);
    }

    void TestFloat()
    {
        ASSERT(1.0f + 1.0f == 2.0f);
    }

    void TestFail()
    {
        ASSERT(1 + 1 == 1);
    }
};



int main()
{
    TEST(MyTest);
    return 0;
}