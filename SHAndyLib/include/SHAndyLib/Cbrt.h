#pragma once

#include <SHAndyLib/Float.h>
#include <SHAndyLib/Sqrt.h>

namespace shandy::detail
{

template<typename T>
T constexpr cbrtNewtonRaphson(const T x, T curr, T prev)
{
    while (not shandy::eq(curr, prev))
    {
        prev = curr;

        curr = (static_cast<T>(2.0 / 3.0) * curr + (x * static_cast<T>(1.0 / 3.0)) / (curr * curr));
    }

    return curr;
}

template<typename T>
T constexpr cbrt(const T x)
{
    if (x < std::numeric_limits<T>::infinity())
    {
        return detail::cbrtNewtonRaphson(x, shandy::sqrt(x) / 2, static_cast<T>(0));
    }
    else
    {
        return std::numeric_limits<T>::quiet_NaN();
    }
}

} // namespace shandy::detail

namespace shandy
{

constexpr auto cbrt(const std::floating_point auto x)
{
    return (x >= 0) ? detail::cbrt(x) : -(detail::cbrt(-x));
}

constexpr auto cbrt(const std::integral auto x)
{
    return cbrt(static_cast<double>(x));
}

} // namespace shandy
