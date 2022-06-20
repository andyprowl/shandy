#pragma once

#include <cstddef>

inline constexpr std::byte operator ""_b(const unsigned long long value)
{
    return static_cast<std::byte>(value);
}

inline constexpr std::size_t operator ""_uz(const unsigned long long value)
{
    return static_cast<std::size_t>(value);
}
