#include "FileSystem.h"

namespace Bored {
namespace FileSystem {
File::File(const std::string& p, const std::string& n, std::vector<char>& c)
    : path(p), name(n), content(c){};

void File::SetName(const std::string& n) { name = n; }

void File::SetPath(const std::string& p) { path = p; }

void File::SetContent(std::vector<char>& c) { content = c; }

}  // namespace FileSystem
}  // namespace Bored
