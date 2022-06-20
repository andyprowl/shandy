#pragma once

#include <SHAndyLib/Float.h>
#include <SHAndyLib/Primes.h>

#include <concepts>
#include <cstddef>

namespace shandy::detail
{

template<typename T>
constexpr double sqrtNewtonRaphson(const T x, T curr, T prev)
{
    while (not shandy::eq(curr, prev))
    {
        prev = curr;
        
        curr = 0.5 * (curr + x / curr);
    }

    return curr;
}

} // namespace shandy::detail

namespace shandy
{

constexpr auto sqrt(const std::floating_point auto x)
{
    using T = decltype(x);

    if ((x >= T(0)) && (x < std::numeric_limits<T>::infinity()))
    {
        return detail::sqrtNewtonRaphson(x, x, static_cast<T>(0));
    }
    else
    {
        return std::numeric_limits<T>::quiet_NaN();
    }
}

constexpr auto sqrt(const std::integral auto x)
{
    return sqrt(static_cast<double>(x));
}

} // namespace shandy
