#pragma once

#include "SubWindow.hpp"

enum class AssetType {
  ALL,
  TEXTURE,
  MATERIAL,
  MESH,
  MODEL
};

// TODO: Use renderer or something to access the assets
class AssetManager : public SubWindow {
public:
  AssetManager(Manifest& game);

  virtual void OnSetup() override;
  virtual bool OnUpdate(double dt) override;
  virtual void OnShutdown() override;
  virtual void NewFrame() override;

  void SetType(const AssetType& type);

private:
  AssetType _type = AssetType::ALL;
};
