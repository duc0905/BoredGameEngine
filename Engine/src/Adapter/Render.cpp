#include "Render.h"

namespace Bored
{
namespace Render
{
unsigned int GetSizeOf(const ComponentType& t)
{
    switch (t)
    {
    case ComponentType::Float:
        return sizeof(float);
    case ComponentType::Float2:
        return sizeof(float) * 2;
    case ComponentType::Float3:
        return sizeof(float) * 3;
    case ComponentType::Float4:
        return sizeof(float) * 4;
    case ComponentType::Int:
        return sizeof(int);
    case ComponentType::Int2:
        return sizeof(int) * 2;
    case ComponentType::Int3:
        return sizeof(int) * 3;
    case ComponentType::Int4:
        return sizeof(int) * 4;
    case ComponentType::Mat3:
        return sizeof(float) * 9;
    case ComponentType::Mat4:
        return sizeof(float) * 16;
    case ComponentType::Bool:
        return sizeof(bool);
    default:
        return 0;
    }
}

uint8_t GetCountOf(const ComponentType& t)
{
    switch (t)
    {
    case ComponentType::Float:
        return 1;
    case ComponentType::Float2:
        return 2;
    case ComponentType::Float3:
        return 3;
    case ComponentType::Float4:
        return 4;
    case ComponentType::Int:
        return 1;
    case ComponentType::Int2:
        return 2;
    case ComponentType::Int3:
        return 3;
    case ComponentType::Int4:
        return 4;
    case ComponentType::Mat3:
        return 9;
    case ComponentType::Mat4:
        return 16;
    case ComponentType::Bool:
        return 1;
    default:
        return 0;
    }
}

unsigned int GetStride(const BufferLayout& bl)
{
    unsigned int stride = 0;

    for (auto& [_, comp] : bl)
    {
        stride += GetSizeOf(comp);
    }

    return stride;
}

} // namespace Render
} // namespace Bored
