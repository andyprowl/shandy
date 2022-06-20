#pragma once

#include <SHAndyLib/Cbrt.h>
#include <SHAndyLib/Primes.h>
#include <SHAndyLib/Sqrt.h>
#include <SHAndyLib/Types.h>

#include <array>
#include <cstddef>

namespace shandy::detail
{
    
inline static constexpr auto primeNumbers = computePrimeNumbers<64>();

constexpr std::uint32_t getFirst32BitsOfFractionalPartOfPrimeSqrt(const int primeIndex)
{
    const auto prime = primeNumbers[primeIndex];

    const auto root = shandy::sqrt(prime);

    const auto fractionalPart = root - static_cast<int>(root);

    return static_cast<std::uint32_t>(fractionalPart * 0x100000000);
}

constexpr std::uint32_t getFirst32BitsOfFractionalPartOfPrimeCbrt(const int primeIndex)
{
    const auto prime = primeNumbers[primeIndex];

    const auto root = shandy::cbrt(prime);

    const auto fractionalPart = root - static_cast<int>(root);

    return static_cast<std::uint32_t>(fractionalPart * 0x100000000);
}

constexpr SHA256 getInitialHash()
{
    auto hash = SHA256{};

    for (auto primeIndex = 0; primeIndex < hash.size(); ++primeIndex)
    {
        hash[primeIndex] = getFirst32BitsOfFractionalPartOfPrimeSqrt(primeIndex);
    }

    return hash;
}

constexpr KConstants getKConstants()
{
    auto constants = KConstants{};

    for (auto primeIndex = 0; primeIndex < constants.size(); ++primeIndex)
    {
        constants[primeIndex] = getFirst32BitsOfFractionalPartOfPrimeCbrt(primeIndex);
    }

    return constants;
}

} // namespace shandy::detail
