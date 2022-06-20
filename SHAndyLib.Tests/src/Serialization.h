#pragma once

#include <filesystem>

namespace shandy
{

void writeToFile(const auto& data, const std::filesystem::path& path)
{
    auto os = std::ofstream{path};

    os.write(data.data(), data.size());
}

std::string readFromFile(const std::filesystem::path& path);

} // namespace shandy
