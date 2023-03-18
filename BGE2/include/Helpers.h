#pragma once

#include "pch.h"

namespace Bored {
struct Image {
  std::string filepath;
  unsigned int width = 0, height = 0, bpp = 4;
  unsigned char *data = nullptr;
  bool loaded = false;
};

Image LoadImage(const std::string& file)
{
  int w, h, n;
  unsigned char *bytes = stbi_load(file.c_str(), &w, &h, &n, 4);
  
  if (bytes == NULL) {
	// ERROR
  }

  return {file, (unsigned int)w, (unsigned int)h, (unsigned int)n, bytes, true};
}
}
