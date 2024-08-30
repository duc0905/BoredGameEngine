#include "Loader.hpp"

#include <exception>
#include <filesystem>
#include <fstream>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <winbase.h>
#include <nlohmann/json.hpp>

#include "Manifest.hpp"

using nlohmann::json;

std::vector<std::string> splitString(std::string s, std::string delim) {
  std::vector<std::string> res;
  int pos = 0;

  // while(pos < s.size()){
  while ((pos = s.find(delim)) != std::string::npos) {
    res.push_back(s.substr(0, pos));
    s.erase(0, pos + delim.length());
  }
  res.push_back(s);
  return res;
}

Manifest::Version ParseVersion(json data) {
  if (!data.contains("version")) {
    throw std::runtime_error("Field 'version' is missing from Bored.json");
  }

  if (!data["version"].is_string()) {
    throw std::runtime_error("Field 'version' must be of type string");
  }

  std::string version = data["version"];
  auto vs = splitString(version, ".");
  Manifest::Version v;
  if (vs.size() == 0) {
    throw std::format_error(
        std::format("Invalid version number: %s", version).c_str());
  }

  v.major = std::stoi(vs[0]);
  if (vs.size() > 1)
    v.minor = std::stoi(vs[1]);
  else
    v.minor = 0;
  if (vs.size() > 2)
    v.patch = std::stoi(vs[2]);
  else
    v.patch = 0;

  return v;
}

std::string ParseName(json data) {
  if (!data.contains("name")) {
    throw std::runtime_error("Field 'name' is missing from Bored.json");
  }

  if (!data["name"].is_string()) {
    throw std::runtime_error("Field 'name' must be of type string");
  }

  return data["name"];
}

std::string ParseDescription(json data) {
  if (!data.contains("description")) {
    throw std::runtime_error("Field 'description' is missing from Bored.json");
  }

  if (!data["description"].is_string()) {
    throw std::runtime_error("Field 'description' must be of type string");
  }

  return data["description"];
}

void LoadModule(std::shared_ptr<Bored::Scene> scene, json data) {
    data.template get<int>();
  if (data.contains("modules")) {
    if (data["modules"].is_array()) {
      for (auto m : data["modules"]) {
        // NOTE: Only support adding modules using dynamic lib for now
        // Built-in modules also will be added usig this way
        if (m.contains("dylib_path")) {
          scene->AddDLModule<Bored::Module>(m["dylib_path"]);
        }
      }
    }
  }
}

// TODO: implement to_json and from_json
// https://json.nlohmann.me/features/arbitrary_types/#basic-usage

void LoadActors(std::shared_ptr<Bored::Scene> scene, json data) {
  if (data.contains("actors")) {
    if (data["actors"].is_array()) {
      auto& am = scene->GetActorManager();
      for (auto a : data["actors"]) {
        auto actor = am.Create<Bored::Actor>();
        actor->name = a["name"];
        // TODO: implement Load components
      }
    }
  }
}

std::shared_ptr<Bored::Scene> ParseScene(std::string path) {
  // TODO: implement
  std::ifstream f(path);

  if (!f.is_open()) {
    std::cerr << "[Warning]: Scene file does not exist: " << path << std::endl;
    return nullptr;
  }

  json data;
  try {
    data = json::parse(f);
  } catch (std::exception e) {
    std::cerr << "[Error]: Error while parsing JSON file: " << e.what()
              << std::endl;
    return nullptr;
  }

  std::shared_ptr<Bored::Scene> scene = std::make_shared<Bored::Scene>();

  scene->m_name = data["name"];
  LoadModule(scene, data);
  LoadActors(scene, data);

  return scene;
}

std::vector<std::shared_ptr<Bored::Scene>> ParseScenes(json data) {
  if (!data.contains("scenes")) {
    return {};
  }

  if (!data["scenes"].is_array()) {
    throw std::runtime_error("Field 'scenes' must be of type array of strings");
  }

  std::vector<std::shared_ptr<Bored::Scene>> scenes;
  for (auto s : data["scenes"]) {
    if (!s.is_string()) {
      throw std::runtime_error(
          "Field 'scenes' must be of type array of strings");
    }

    auto scene = ParseScene(s);
    if (scene != nullptr) {
      scenes.push_back(scene);
    }
  }

  return scenes;
}

Manifest LoadManifestFile(const std::string& file) {
  Manifest m;
  std::ifstream f(file);

  if (!f.is_open()) {
    std::cerr << "Manifest file DNE\n";
    return {};
  }

  json data;
  try {
    data = json::parse(f);
  } catch (std::exception e) {
    std::cerr << "Error: " << e.what() << std::endl;
    throw std::runtime_error(
        std::format("Cannot parse manifest file: %s", file));
  }

  m.name = ParseName(data);
  m.description = ParseDescription(data);
  m.version = ParseVersion(data);

  m.scenes = ParseScenes(data);

  // NOTE: path cannot be stored in the manifest file
  // path will be set by the editor when open a project
  /* TODO: Implement other fields */

  return m;
}

void PrintManifest(const Manifest& game) {
  std::cout << "Name: " << game.name << std::endl
            << "Description: " << game.description << std::endl
            << "Version: " << game.version.major << "." << game.version.minor
            << "." << game.version.patch << std::endl;
  std::cout << "Core modules: \n";
  for (auto x : game.core_modules) std::cout << x.first << std::endl;

  std::cout << "Custom modules: \n";
  for (auto x : game.custom_modules) std::cout << x.first << std::endl;

  std::cout << "Path: " << game.path << std::endl;
}
