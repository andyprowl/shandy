#pragma once

#include <array>
#include <cstddef>
#include <numeric>
#include <vector>

namespace shandy
{

template<std::size_t N>
constexpr std::array<int, N> computePrimeNumbers()
{
    const auto sieveLimit = N * N;

    auto integers = std::vector<int>(sieveLimit);

    std::iota(std::begin(integers), std::end(integers), 0);

    auto marks = std::vector<std::uint8_t>(sieveLimit, 0);

    auto primes = std::array<int, N>{};

    auto numOfPrimes = 0;

    for (auto n = 2; n < sieveLimit; ++n)
    {
        if (marks[n])
        {
            continue;
        }

        marks[n] = 1;

        primes[numOfPrimes] = n;

        if (++numOfPrimes == N)
        {
            break;
        }

        for (auto i = n * 2; i < sieveLimit; i += n)
        {
            marks[i] = 1;
        }
    }
        
    return primes;
}

} // namespace shandy
