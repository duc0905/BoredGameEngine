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
    struct Line {
        glm::vec3 from, to;
        glm::vec4 colorFrom, colorTo;
        bool hide = false;

        Line(const glm::vec3& _from, const glm::vec3& _to, const glm::vec4& color1, const glm::vec4& color2)
            : from(_from), to(_to), colorFrom(color1), colorTo(color2), hide(false){}
        Line(const glm::vec3& _from, const glm::vec3& _to, const glm::vec4& color)
            : Line(_from, _to, color, color) {}
        Line(const glm::vec3& _from, const glm::vec3& _to)
            : Line(_from, _to, {1,1,1,1}) {}
    };
    static std::shared_ptr<IRenderer> GetDefault()
    {
        return defaultRenderer_;
    }

    virtual ~IRenderer() {};
    virtual void Render(IWorld& world) = 0;

    virtual void Draw(const Actor& actor) = 0;
    virtual void DrawLine(Line& line) = 0;
    virtual void DrawLine(const Line& line) = 0;
    virtual unsigned int GetMouseHoverEntityID(int x, int y) = 0;
    virtual void UseCamera(std::shared_ptr<BaseCamera> cam) = 0;
};

class NullRenderer : public IRenderer
{
	// Inherited via IRenderer
	inline virtual void Render(IWorld& world) override {};
	inline virtual void Draw(const Actor& actor) override {};
    virtual void DrawLine(Line& line) {};
    virtual void DrawLine(const Line& line) {};
	inline virtual void UseCamera(std::shared_ptr<BaseCamera> cam) override {};

    // Inherited via IRenderer
    virtual unsigned int GetMouseHoverEntityID(int x, int y) override {
        return 0;
    }
};
