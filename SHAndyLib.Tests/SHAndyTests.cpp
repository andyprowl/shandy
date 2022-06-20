#include "pch.h"

#include "Serialization.h"

#include <SHAndyLib/Formatting.h>
#include <SHAndyLib/SHA256.h>
#include <SHAndyLib/UDL.h>

#include <CppUnitTest.h>

#include <bitset>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace shandy
{

TEST_CLASS(SHAndy)
{

public:
        
    // Refernce results obtained from https://sha256algorithm.com/

    TEST_METHOD(Step1_Short)
    {
        const auto block = shandy::detail::makeMessageBlock("hello, world!");

        Assert::AreEqual(64_uz, block.size());

        shandy::writeToFile(formatBinary(block), R"(c:\temp\step1-short)");
    }

    TEST_METHOD(Step1_Long)
    {
        static constexpr auto msg = shandy::asUint8Array(
            "hello i am a really bored software engineer "
            "and I am considering doing this thing for fun");

        const auto block = shandy::detail::makeMessageBlock(msg);

        Assert::AreEqual(128_uz, block.size());

        shandy::writeToFile(shandy::formatBinary(block), R"(c:\temp\step1-long)");
    }

    TEST_METHOD(Step2_Short)
    {
        const auto block = shandy::detail::makeMessageBlock("hello, world!");

        const auto messageSchedule = shandy::detail::makeMessageSchedule(block, 0);

        Assert::AreEqual(64_uz, messageSchedule.size());

        shandy::writeToFile(formatBinary(messageSchedule), R"(c:\temp\step2-short)");
    }

    TEST_METHOD(CalculateSHA256_Short)
    {
        const auto hash = shandy::calculateSHA256("hello, world!");

        Assert::AreEqual(8_uz, hash.size());

        Assert::AreEqual(0x68e656b2u, hash[0]);

        Assert::AreEqual(0x51e67e83u, hash[1]);

        Assert::AreEqual(0x58bef848u, hash[2]);

        Assert::AreEqual(0x3ab0d51cu, hash[3]);

        Assert::AreEqual(0x6619f3e7u, hash[4]);

        Assert::AreEqual(0xa1a9f0e7u, hash[5]);

        Assert::AreEqual(0x5838d41fu, hash[6]);

        Assert::AreEqual(0xf368f728u, hash[7]);
    }

    TEST_METHOD(CalculateSHA256_Long)
    {
        const auto hash = shandy::calculateSHA256(
            "hello i am a really bored software engineer "
            "and I am considering doing this thing for fun");

        Assert::AreEqual(8_uz, hash.size());

        Assert::AreEqual(0x7064baebu, hash[0]);

        Assert::AreEqual(0x3c367fecu, hash[1]);

        Assert::AreEqual(0x1f4c8ef7u, hash[2]);

        Assert::AreEqual(0x631958cau, hash[3]);

        Assert::AreEqual(0x2638a27eu, hash[4]);

        Assert::AreEqual(0x82cabec2u, hash[5]);

        Assert::AreEqual(0x5ddf693du, hash[6]);

        Assert::AreEqual(0x74079c30u, hash[7]);
    }
    
    TEST_METHOD(CalculateSHA256_Long_CharPointer)
    {
        const auto msg = 
            "hello i am a really bored software engineer "
            "and I am considering doing this thing for fun";

        const auto hash = shandy::calculateSHA256(msg);

        Assert::AreEqual(8_uz, hash.size());

        Assert::AreEqual(0x7064baebu, hash[0]);

        Assert::AreEqual(0x3c367fecu, hash[1]);

        Assert::AreEqual(0x1f4c8ef7u, hash[2]);

        Assert::AreEqual(0x631958cau, hash[3]);

        Assert::AreEqual(0x2638a27eu, hash[4]);

        Assert::AreEqual(0x82cabec2u, hash[5]);

        Assert::AreEqual(0x5ddf693du, hash[6]);

        Assert::AreEqual(0x74079c30u, hash[7]);
    }

    TEST_METHOD(CalculateSHA256_Long_Constexpr)
    {
        static constexpr auto hash = shandy::calculateSHA256(
            u8"hello i am a really bored software engineer "
            u8"and I am considering doing this thing for fun");

        static_assert(8_uz == hash.size());

        static_assert(0x7064baebu == hash[0]);

        static_assert(0x3c367fecu == hash[1]);

        static_assert(0x1f4c8ef7u == hash[2]);

        static_assert(0x631958cau == hash[3]);

        static_assert(0x2638a27eu == hash[4]);

        static_assert(0x82cabec2u == hash[5]);

        static_assert(0x5ddf693du == hash[6]);

        static_assert(0x74079c30u == hash[7]);
    }
};

} // namespace shandy
