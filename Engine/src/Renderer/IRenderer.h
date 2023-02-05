#pragma once
#include "../pch.h"
#include "../System.h"
#include "../World/World.h"
#include "../Actor/BaseCamera.h"

#include "Mesh.h"
#include "ShaderClass.h"

class IRenderer :
    public System
{
    static std::shared_ptr<IRenderer> defaultRenderer_;
public:
    static std::shared_ptr<IRenderer> GetDefault()
    {
        return defaultRenderer_;
    }

    virtual ~IRenderer() {};
    virtual void Render(IWorld& world) = 0;

    virtual void Draw(const Actor& actor) = 0;
    virtual void DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color) = 0;
    virtual unsigned int GetMouseHoverEntityID(int x, int y) = 0;
    virtual void UseCamera(std::shared_ptr<BaseCamera> cam) = 0;
};

class NullRenderer : public IRenderer
{
	// Inherited via IRenderer
	inline virtual void Render(IWorld& world) override {};
	inline virtual void Draw(const Actor& actor) override {};
    inline virtual void DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color) {};
	inline virtual void UseCamera(std::shared_ptr<BaseCamera> cam) override {};

    // Inherited via IRenderer
    virtual unsigned int GetMouseHoverEntityID(int x, int y) override {
        return 0;
    }
};
