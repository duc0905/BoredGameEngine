#pragma once
#include <string>
#include <map>
#include <memory>
#include <Engine/Scene.hpp>

struct Manifest {
  /* These are from Bored.json */
  std::string name;
  std::string description;

  struct Version {
    unsigned short major;
    unsigned short minor;
    unsigned short patch;
  } version;

  // Map Name/Token -> Module
  // WARN: Deprecated. Each scene has its own modules now
  std::map<std::string, std::shared_ptr<Bored::Module>> core_modules;
  std::map<std::string, std::shared_ptr<Bored::Module>> custom_modules;

  std::vector<std::shared_ptr<Bored::Scene>> scenes;

  /* Others */
  std::string path;
};
