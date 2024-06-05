#include "Bored.hpp"
#include <string>
#include <vector>

struct Manifest {
  std::string name;
  std::string description;
  struct {
    unsigned short major;
    unsigned short minot;
    unsigned short patch;
  } version;
  std::vector<Bored::Module> core_modules;
  std::vector<Bored::Module> custom_modules;
};
