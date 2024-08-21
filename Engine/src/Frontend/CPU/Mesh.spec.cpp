#include "Adapter/Render.h"
#include <glm/fwd.hpp>
#include <gtest/gtest.h>
#include <Frontend/CPU/Mesh.hpp>

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

namespace CPUUtilTest
{
class MeshTest : public testing::Test
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
        mesh = Bored::Render::CPU::Mesh();
        mesh.name = "Test mesh";
        mesh.SubPos({
            {0.1f, 0.2f, 0.3f},
            {0.4f, 0.5f, 0.6f},
            {0.7f, 0.8f, 0.9f},
        });
        mesh.SubUVs({{0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}});
        mesh.SubNorms({
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f},
        });
        mesh.SubIndices({0, 1, 2, 3, 4});
    }

    void TearDown() override
    {
    }

    Bored::Render::CPU::Mesh mesh;
};

TEST_F(MeshTest, GetCorrectPosition)
{
    std::vector<glm::vec3> pos = mesh.GetPos();

    EXPECT_EQ(pos.size(), 3);
    EXPECT_EQ(pos, std::vector<glm::vec3>({
                       {0.1f, 0.2f, 0.3f},
                       {0.4f, 0.5f, 0.6f},
                       {0.7f, 0.8f, 0.9f},
                   }));
}

TEST_F(MeshTest, GetCorrectUVS)
{
    std::vector<glm::vec2> uvs = mesh.GetUVs();

    EXPECT_EQ(uvs.size(), 3);
    EXPECT_EQ(uvs, std::vector<glm::vec2>({{0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}}));
}

TEST_F(MeshTest, GetCorrectNorm)
{
    std::vector<glm::vec3> norm = mesh.GetNorms();

    EXPECT_EQ(norm.size(), 3);
    EXPECT_EQ(norm, std::vector<glm::vec3>({
                        {0.0f, 0.0f, 0.0f},
                        {1.0f, 1.0f, 1.0f},
                        {0.0f, 1.0f, 0.0f},
                    }));
}

TEST_F(MeshTest, GetCorrectIndices)
{
    std::vector<unsigned int> indices = mesh.GetIndices();

    EXPECT_EQ(indices.size(), 5);
    EXPECT_EQ(indices, std::vector<unsigned int>({0, 1, 2, 3, 4}));
}
} // namespace CPUUtilTest
