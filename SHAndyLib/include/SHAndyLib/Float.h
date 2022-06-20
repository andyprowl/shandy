#pragma once

#include <climits>

namespace shandy
{

template<typename T>
bool constexpr eq(const T x, const T y, const T epsilon = std::numeric_limits<T>::epsilon())
{
    return (x < y) ? (y - x) <= y * epsilon : (x - y) <= x * epsilon;
}

} // namespace shandy
