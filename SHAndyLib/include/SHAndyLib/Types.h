#pragma once

#include <array>
#include <cstddef>

namespace shandy::detail
{

using KConstants = std::array<std::uint32_t, 64>;

using MessageSchedule = std::array<std::uint32_t, 64>;

using WorkingVariables = std::array<std::uint32_t, 8>;

} // namespace shandy::detail

namespace shandy
{

using SHA256 = std::array<std::uint32_t, 8>;

} // namespace shandy
