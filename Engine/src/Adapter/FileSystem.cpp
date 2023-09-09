#include "FileSystem.h"

namespace Bored {
namespace FileSystem {
File::File(const std::string& p, const std::string& n)
    : path(p), name(n){};

File::File() : path(""), name(""){};

void File::SetName(const std::string& n) { name = n; }

void File::SetPath(const std::string& p) { path = p; }

}  // namespace FileSystem
}  // namespace Bored
