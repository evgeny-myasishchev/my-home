#include <gtest/gtest.h>
#include <array-ptr.h>

namespace
{

TEST(v2ArrayPtr, size)
{
    int values[4] = {1, 2, 3, 4};

    v2::ArrayPtr<int> ptr(4, values);
    ASSERT_EQ(4, ptr.size());
}

TEST(v2ArrayPtr, subscript)
{
    int values[4] = {1, 2, 3, 4};

    v2::ArrayPtr<int> ptr(4, values);
    for (size_t i = 0; i < ptr.size(); i++)
    {
        ASSERT_EQ(values[i], ptr[i]);
    }

    ASSERT_EQ(0, ptr[ptr.size() + 1]);
    ASSERT_EQ(0, ptr[-1]);
}

} // namespace
