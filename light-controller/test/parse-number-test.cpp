#include "gtest/gtest.h"
#include "test-lib/random.h"
#include "numerics.h"
#include <string>
#include <logger.h>

namespace
{
    TEST(parseNumber, validNumber)
    {
        const int value = test::randomNumber(0, 20000);
        const auto strVal = std::to_string(value);
        auto result = parseNumber(strVal.c_str(), strVal.length());
        ASSERT_EQ(result.value, value);
        ASSERT_EQ(result.valid, true);
    }

    TEST(parseNumber, invalidNumber)
    {
        const int value = test::randomNumber(0, 20000);
        const auto strVal = std::to_string(value) + "ccc";
        auto result = parseNumber(strVal.c_str(), strVal.length());
        ASSERT_EQ(result.valid, false);
    }
}