#include <gtest/gtest.h>

#include <Adapter/GLFWWindow.h>
#include <Adapter/OGL.h>
#include <Frontend/RenderUtil.hpp>
#include <Frontend/CPU/Factory.hpp>
#include <Frontend/OGL/Factory.hpp>

namespace
{
typedef Bored::Render::IFactory* CreateFactoryFunc();

class FactoryTest : public ::testing::TestWithParam<CreateFactoryFunc*>
{
  protected:
    FactoryTest()
    {
    }

    ~FactoryTest() override
    {
    }

    void SetUp() override
    {
        if (!setup)
        {
            window = Bored::GLFW::Window::GetInstance();
            window->OnSetup();
            window->UseRenderContext(Bored::Render::OGL::Context::GetDefault());
            setup = true;
        }

        factory = GetParam()();
    }

    void TearDown() override
    {
        delete factory;
    }

    Bored::Render::IFactory* factory;

    static Bored::Window* window;
    static bool setup;
};

bool FactoryTest::setup = false;
Bored::Window* FactoryTest::window = nullptr;

TEST_P(FactoryTest, CreateNotNullMesh)
{
    EXPECT_NE(factory->CreateMesh(), nullptr);
}

Bored::Render::IFactory* CreateCPUFactory() { return new Bored::Render::CPU::Factory(); }
Bored::Render::IFactory* CreateOGLFactory() { return new Bored::Render::OGL::Factory(); }

INSTANTIATE_TEST_SUITE_P(CPUandOGL, FactoryTest, testing::Values(CreateCPUFactory, CreateOGLFactory));

}
