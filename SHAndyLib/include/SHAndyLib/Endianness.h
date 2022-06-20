#pragma once

#include <bit>
#include <cstddef>

namespace shandy
{

constexpr bool isLittleEndian()
{
    return (std::endian::native == std::endian::little);
}

constexpr void storeAsBigEndian(const std::uint64_t value, std::uint8_t* const dest)
{
    constexpr auto baseMask = static_cast<std::uint64_t>(0xff);

    if constexpr (isLittleEndian())
    {
        dest[7] = static_cast<std::uint8_t>((value & (baseMask << 0)) >> 0);
        dest[6] = static_cast<std::uint8_t>((value & (baseMask << 8)) >> 8);
        dest[5] = static_cast<std::uint8_t>((value & (baseMask << 16)) >> 16);
        dest[4] = static_cast<std::uint8_t>((value & (baseMask << 24)) >> 24);
        dest[3] = static_cast<std::uint8_t>((value & (baseMask << 32)) >> 32);
        dest[2] = static_cast<std::uint8_t>((value & (baseMask << 40)) >> 40);
        dest[1] = static_cast<std::uint8_t>((value & (baseMask << 48)) >> 48);
        dest[0] = static_cast<std::uint8_t>((value & (baseMask << 56)) >> 56);
    }
    else
    {
        dest[0] = static_cast<std::uint8_t>((value & (baseMask << 0)) >> 0);
        dest[1] = static_cast<std::uint8_t>((value & (baseMask << 8)) >> 8);
        dest[2] = static_cast<std::uint8_t>((value & (baseMask << 16)) >> 16);
        dest[3] = static_cast<std::uint8_t>((value & (baseMask << 24)) >> 24);
        dest[4] = static_cast<std::uint8_t>((value & (baseMask << 32)) >> 32);
        dest[5] = static_cast<std::uint8_t>((value & (baseMask << 40)) >> 40);
        dest[6] = static_cast<std::uint8_t>((value & (baseMask << 48)) >> 48);
        dest[7] = static_cast<std::uint8_t>((value & (baseMask << 56)) >> 56);
    }
}

constexpr std::uint32_t makeUInt32(const std::uint8_t* const start)
{
    if constexpr (isLittleEndian())
    {
        return (start[3] | (start[2] << 8) | (start[1] << 16) | (start[0] << 24));
    }
    else
    {
        return (start[0] | (start[1] << 8) | (start[2] << 16) | (start[3] << 24));
    }
}

} // namespace shandy
