#include "Serialization.h"

#include <fstream>
#include <sstream>

namespace shandy
{

std::string readFromFile(const std::filesystem::path& path)
{
    auto s = std::ostringstream{};

    s << std::ifstream{path}.rdbuf();

    return s.str();
}

} // namespace shandy
