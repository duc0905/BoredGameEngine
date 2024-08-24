#include "Adapter/Render.h"
#include "Adapter/Window.h"
#include "gtest/gtest.h"
#include <glm/fwd.hpp>
#include <gtest/gtest.h>

#include <Adapter/GLFWWindow.h>
#include <Frontend/CPU/Mesh.hpp>
#include <Frontend/OGL/Mesh.hpp>

TEST(SimpleCPUMesh, GetVAO)
{
    Bored::Render::CPU::Mesh mesh;
    Bored::Render::VertexArray* vao = mesh.GetVertexArray();
    ASSERT_NE(vao, nullptr);
}

TEST(SimpleCPUMesh, GetEBO)
{
    Bored::Render::CPU::Mesh mesh;
    Bored::Render::IndexBuffer* ebo = mesh.GetIndexBuffer();
    ASSERT_NE(ebo, nullptr);

    EXPECT_EQ(ebo->GetSize(), 0);
    EXPECT_EQ(ebo->GetData().size(), 0);
}

namespace RenderUtilTest
{

typedef Bored::Render::IMesh* CreateMeshFunc();

class MeshTest : public testing::TestWithParam<CreateMeshFunc*>
{
  protected:
    MeshTest()
    {
    }

    ~MeshTest() override
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

        mesh = GetParam()();
        mesh->name = "Test mesh";
        mesh->SubPos({
            {0.1f, 0.2f, 0.3f},
            {0.4f, 0.5f, 0.6f},
            {0.7f, 0.8f, 0.9f},
        });
        mesh->SubUVs({{0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}});
        mesh->SubNorms({
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f},
        });
        mesh->SubIndices({0, 1, 2, 3, 4});
    }

    void TearDown() override
    {
        delete mesh;
    }

    Bored::Render::IMesh* mesh;
    Bored::Window* window;
    static bool setup;
};

bool MeshTest::setup = false;

TEST_P(MeshTest, GetCorrectPosition)
{
    std::vector<glm::vec3> pos = mesh->GetPos();

    EXPECT_EQ(pos.size(), 3);
    EXPECT_EQ(pos, std::vector<glm::vec3>({
                       {0.1f, 0.2f, 0.3f},
                       {0.4f, 0.5f, 0.6f},
                       {0.7f, 0.8f, 0.9f},
                   }));
}

TEST_P(MeshTest, GetCorrectUVS)
{
    std::vector<glm::vec2> uvs = mesh->GetUVs();

    EXPECT_EQ(uvs.size(), 3);
    EXPECT_EQ(uvs, std::vector<glm::vec2>({{0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}}));
}

TEST_P(MeshTest, GetCorrectNorm)
{
    std::vector<glm::vec3> norm = mesh->GetNorms();

    EXPECT_EQ(norm.size(), 3);
    EXPECT_EQ(norm, std::vector<glm::vec3>({
                        {0.0f, 0.0f, 0.0f},
                        {1.0f, 1.0f, 1.0f},
                        {0.0f, 1.0f, 0.0f},
                    }));
}

TEST_P(MeshTest, GetCorrectIndices)
{
    std::vector<unsigned int> indices = mesh->GetIndices();

    EXPECT_EQ(indices.size(), 5);
    EXPECT_EQ(indices, std::vector<unsigned int>({0, 1, 2, 3, 4}));
}

Bored::Render::IMesh* CreateCPUMesh()
{
    return new Bored::Render::CPU::Mesh();
}
Bored::Render::IMesh* CreateOGLMesh()
{
    return new Bored::Render::OGL::Mesh();
}
INSTANTIATE_TEST_SUITE_P(CPUandOGL, MeshTest, testing::Values(&CreateCPUMesh, &CreateOGLMesh));
} // namespace RenderUtilTest
