#pragma once

#include <entt/meta/factory.hpp>

// Macro for creating a helper function for registering component memebers to
// the entt meta system
#define SETUP_COMPONENT(Comp)                                                  \
public:                                                                        \
  template <auto data> static void RegisterProp(const std::string &name) {     \
    auto name_hash = entt::hashed_string(name.c_str());                        \
    entt::meta_factory<Comp>{}                                                 \
        .data<data>(name_hash)                                                 \
        .template custom<std::string>(name);                                   \
  }                                                                            \
  static void Register()

// TODO: Find out how to handle "resource" fields for the meta system.
// TODO Component list:
//  - InputComponent
//  - MeshComponent
//  - CameraComponent
