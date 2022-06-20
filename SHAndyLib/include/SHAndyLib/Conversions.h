#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace shandy
{

template<typename T>
concept CharType = requires()
{
    typename std::char_traits<T>::char_type;
};

template<typename T, std::size_t N>
    requires CharType<T>
constexpr std::array<std::uint8_t, N - 1> asUint8Array(const T (&data)[N])
{
    auto result = std::array<std::uint8_t, N - 1>{};

    for (auto i = 0_uz; i < N - 1; ++i)
    {
        result[i] = static_cast<std::uint8_t>(data[i]);
    }

    return result;
}

constexpr std::vector<std::uint8_t> asUint8Vector(const auto& data)
{
    auto result = std::vector<std::uint8_t>{};

    result.reserve(data.size());

    for (const auto& elem : data)
    {
        result.push_back(static_cast<std::uint8_t>(elem));
    }

    return result;
}

template<typename T>
    requires CharType<T>
constexpr std::vector<std::uint8_t> asUint8Vector(const T* const s)
{
    return asUint8Vector(std::basic_string_view<T>{s});
}

} // namespace shandy
