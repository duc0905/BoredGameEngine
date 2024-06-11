#include "Bored.hpp"
#include <string>
#include <vector>

struct Manifest {
  /* These are from Bored.json */
  std::string name;
  std::string description;

  struct {
    unsigned short major;
    unsigned short minot;
    unsigned short patch;
  } version;

  // Map Name/Token -> Module
  std::map<std::string, Bored::Module> core_modules;
  std::map<std::string, Bored::Module> custom_modules;

  /* Others */
  std::string path;
};
