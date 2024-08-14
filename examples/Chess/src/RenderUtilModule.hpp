#pragma once
#include <map>
#include <memory>
#include "Bored.hpp"

class RenderUtilModule : public Bored::Module
{
  public:
    RenderUtilModule();
    RenderUtilModule(RenderUtilModule&&) = default;
    RenderUtilModule(const RenderUtilModule&) = default;
    RenderUtilModule& operator=(RenderUtilModule&&) = default;
    RenderUtilModule& operator=(const RenderUtilModule&) = default;
    ~RenderUtilModule();

  public:
    virtual void OnSetup();
    virtual void OnSwitchScene();
    virtual bool OnUpdate(double dt);
    virtual void OnShutdown();

  private:
    std::map<std::shared_ptr<Bored::Render::IMesh>, std::shared_ptr<Bored::Render::OGLMesh>> m_meshMap;
};
