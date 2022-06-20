#pragma once

#include <SHAndyLib/SHA256.h>
#include <SHAndyLib/UDL.h>

#include <bitset>
#include <cstddef>
#include <string>
#include <sstream>
#include <vector>

namespace shandy::detail
{

constexpr std::uint8_t getComponent(const std::integral auto n, const int componentIndex)
{
    constexpr auto maskBase = static_cast<std::uint64_t>(0xff);

    const auto shifter = 8 * componentIndex;

    const auto mask = maskBase << shifter;

    return static_cast<std::uint8_t>((n & mask) >> shifter);
}

} // namespace shandy::detail

namespace shandy
{

template<typename Container>
    requires(std::is_same_v<typename Container::value_type, std::uint8_t>)
std::string formatBinary(const Container& cont, const std::size_t numOfColumns = 4)
{
    auto os = std::ostringstream{};

    for (auto i = 0_uz; i < cont.size(); ++i)
    {
        os << std::bitset<8>(cont[i]) << ' ';

        if ((i % numOfColumns) == (numOfColumns - 1))
        {
            os << '\n';
        }
    }

    return os.str();
}

template<typename Container>
    requires(std::is_same_v<typename Container::value_type, std::uint32_t>)
std::string formatBinary(const Container& cont)
{
    auto os = std::ostringstream{};

    for (const auto elem : cont)
    {
        os << std::bitset<32>(elem);

        os << '\n';
    }

    return os.str();
}

inline std::string formatHex(const SHA256& hash)
{
    auto os = std::ostringstream{};

    for (const auto elem : hash)
    {
        const auto h1 = static_cast<int>(detail::getComponent(elem, 3));

        const auto h2 = static_cast<int>(detail::getComponent(elem, 2));

        const auto h3 = static_cast<int>(detail::getComponent(elem, 1));

        const auto h4 = static_cast<int>(detail::getComponent(elem, 0));

        os << std::hex << h1 << " " << h2 << " " << h3 << " " << h4 << " ";
    }

    return os.str();
}

} // namespace shandy
