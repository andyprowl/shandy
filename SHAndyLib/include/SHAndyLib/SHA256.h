#pragma once

#include <SHAndyLib/Conversions.h>
#include <SHAndyLib/Endianness.h>
#include <SHAndyLib/Initialization.h>
#include <SHAndyLib/Math.h>
#include <SHAndyLib/SHA256.h>
#include <SHAndyLib/Types.h>
#include <SHAndyLib/UDL.h>

#include <algorithm>
#include <bitset>
#include <cstddef>
#include <array>
#include <cmath>
#include <iterator>
#include <limits>
#include <numeric>
#include <ranges>
#include <span>
#include <string_view>
#include <type_traits>
#include <vector>

namespace shandy::detail
{

static constexpr auto chunkLengthInBits = 512_uz;

static constexpr auto chunkLengthInBytes = chunkLengthInBits / 8;

template<typename T>
constexpr T expandToSmallestMultipleOf(const T n, const T base)
{
    const auto div = (n / base);

    const auto floor = div * base;
    
    return (n == floor) ? floor : floor + base;
}

constexpr std::vector<std::uint8_t> makeMessageBlock(const std::span<const std::uint8_t> message)
{
    constexpr auto hash = detail::getKConstants();

    constexpr auto sizeLengthInBits = 64_uz;

    const auto messageLengthInBits = std::uint64_t{message.size() * 8};

    const auto payloadLengthInBits = messageLengthInBits + sizeLengthInBits + 1;

    const auto resultLengthInBits = detail::expandToSmallestMultipleOf(payloadLengthInBits, chunkLengthInBits);

    const auto resultLengthInBytes = resultLengthInBits / 8;

    auto result = std::vector<std::uint8_t>(resultLengthInBytes, std::uint8_t{0x0});

    std::ranges::copy(message, std::begin(result));

    result[message.size()] = static_cast<std::uint8_t>(0x1 << 7);

    storeAsBigEndian(messageLengthInBits, result.data() + resultLengthInBytes - 8);

    return result;
}

constexpr std::vector<std::uint8_t> makeMessageBlock(const std::string_view message)
{
    return detail::makeMessageBlock(asUint8Vector(message));
}

constexpr std::uint32_t computeMessageScheduleEntry(
    const MessageSchedule& schedule,
    const std::size_t index)
{
    const auto w0 = schedule[index];

    const auto w1 = schedule[index + 1];

    const auto w9 = schedule[index + 9];

    const auto w14 = schedule[index + 14];

    const auto sigma0 = std::rotr(w1, 7) ^ std::rotr(w1, 18) ^ (w1 >> 3);

    const auto sigma1 = std::rotr(w14, 17) ^ std::rotr(w14, 19) ^ (w14 >> 10);

    return (w0 + sigma0 + w9 + sigma1);
}

constexpr MessageSchedule makeMessageSchedule(
    const std::vector<std::uint8_t>& message,
    const std::size_t chunkIndex)
{
    auto messageSchedule = MessageSchedule();

    const auto start = message.data() + chunkIndex * chunkLengthInBytes;
    
    for (auto i = 0_uz; i < 16_uz; ++i)
    {
        messageSchedule[i] = shandy::makeUInt32(start + i * 4);
    }

    for (auto i = 16_uz; i < 64_uz; ++i)
    {
        messageSchedule[i] = detail::computeMessageScheduleEntry(messageSchedule, i - 16);
    }

    return messageSchedule;
}

constexpr void updateWorkingVariables(
    const std::uint32_t messageScheduleWord,
    const std::uint32_t kConstant,
    WorkingVariables& workingVariables)
{
    const auto a = workingVariables[0];
    const auto b = workingVariables[1];
    const auto c = workingVariables[2];
    const auto d = workingVariables[3];
    const auto e = workingVariables[4];
    const auto f = workingVariables[5];
    const auto g = workingVariables[6];
    const auto h = workingVariables[7];

    const auto sigma1 = std::rotr(e, 6) ^ std::rotr(e, 11) ^ std::rotr(e, 25);
    const auto choice = (e & f) ^ (~e & g);
    const auto temp1 = h + sigma1 + choice + kConstant + messageScheduleWord;

    const auto sigma0 = std::rotr(a, 2) ^ std::rotr(a, 13) ^ std::rotr(a, 22);
    const auto majority = (a & b) ^ (a & c) ^ (b & c);
    const auto temp2 = sigma0 + majority;

    workingVariables[7] = g;
    workingVariables[6] = f;
    workingVariables[5] = e;
    workingVariables[4] = d + temp1;
    workingVariables[3] = c;
    workingVariables[2] = b;
    workingVariables[1] = a;
    workingVariables[0] = temp1 + temp2;
}

constexpr void updateHash(const MessageSchedule& messageSchedule, const KConstants& kConstants, SHA256& hash)
{
    auto workingVariables = hash;

    for (auto i = 0; i < 64; ++i)
    {
        detail::updateWorkingVariables(messageSchedule[i], kConstants[i], workingVariables);
    }

    for (auto i = 0; i < 8; ++i)
    {
        hash[i] += workingVariables[i];
    }
}

} // namespace shandy::detail

namespace shandy
{

constexpr SHA256 calculateSHA256(const std::span<const std::uint8_t>& message)
{
    constexpr auto kConstants = detail::getKConstants();

    auto hash = detail::getInitialHash();

    const auto block = detail::makeMessageBlock(message);

    for (auto i = 0; i < block.size() / 64; ++i)
    {
        const auto messageSchedule = detail::makeMessageSchedule(block, i);

        detail::updateHash(messageSchedule, kConstants, hash);
    }

    return hash;
}

template<typename T>
    requires CharType<T>
constexpr SHA256 calculateSHA256(const std::basic_string_view<T>& message)
{
    const auto v = asUint8Vector(message);

    return shandy::calculateSHA256(v);
}

template<typename T>
    requires CharType<T>
constexpr SHA256 calculateSHA256(const std::basic_string<T>& message)
{
    const auto v = asUint8Vector(message);

    return shandy::calculateSHA256(v);
}

template<typename T, std::size_t N>
    requires CharType<T>
constexpr SHA256 calculateSHA256(const T (&&message)[N])
{
    const auto a = asUint8Array(message);

    return shandy::calculateSHA256(a);
}

template<typename T>
    requires CharType<T>
constexpr SHA256 calculateSHA256(const T* const message)
{
    const auto sv = std::basic_string_view<T>(message);

    return shandy::calculateSHA256(sv);
}

} // namespace shandy
