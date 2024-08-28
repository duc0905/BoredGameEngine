#pragma once

#include "Manifest.hpp"
#include <string>

Manifest LoadManifestFile(const std::string& file);

void PrintManifest(const Manifest& game);
