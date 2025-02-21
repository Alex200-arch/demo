#include <gtest/gtest.h>
#include "../decimal_base.hpp"

// ---------------------- ShortDecimal Tests ----------------------

TEST(ShortDecimalTest, FromIntegerValid) {
    auto sd1 = ShortDecimal<4>::FromInteger(123);
    EXPECT_EQ(sd1.GetRaw(), 1230000);
    auto sd2 = ShortDecimal<4>::FromInteger(-123);
    EXPECT_EQ(sd2.GetRaw(), -1230000);
}

TEST(ShortDecimalTest, FromIntegerOverflow) {
    int64_t max_int = INT64_MAX / 10000;
    EXPECT_NO_THROW(ShortDecimal<4>::FromInteger(max_int));
    int64_t overflow_max_int = max_int + 1;
    EXPECT_THROW(ShortDecimal<4>::FromInteger(overflow_max_int), std::overflow_error);
    int64_t min_int = INT64_MIN / 10000;
    EXPECT_NO_THROW(ShortDecimal<4>::FromInteger(min_int));
    int64_t overflow_min_int = min_int -1;
    EXPECT_THROW(ShortDecimal<4>::FromInteger(overflow_min_int), std::overflow_error);
}

TEST(ShortDecimalTest, FromDoubleValid) {
    auto sd1 = ShortDecimal<4>::FromDouble(123.4567);
    EXPECT_EQ(sd1.GetRaw(), 1234567);
    auto sd2= ShortDecimal<4>::FromDouble(-123.4567);
    EXPECT_EQ(sd2.GetRaw(), -1234567);
}

TEST(ShortDecimalTest, FromDoubleOverflow) {
    double overflow_double1 = static_cast<double>(INT64_MAX) + 1e10;
    EXPECT_THROW(ShortDecimal<4>::FromDouble(overflow_double1), std::overflow_error);
    double overflow_double2 = static_cast<double>(INT64_MIN) - 1e10;
    EXPECT_THROW(ShortDecimal<4>::FromDouble(overflow_double2), std::overflow_error);
}

TEST(ShortDecimalTest, FromStringValid) {
    auto sd1 = ShortDecimal<4>::FromString("123.4567");
    EXPECT_EQ(sd1.GetRaw(), 1234567);
    auto sd2 = ShortDecimal<4>::FromString("+123.0567");
    EXPECT_EQ(sd2.GetRaw(), 1230567);
    auto sd3 = ShortDecimal<4>::FromString("+123.4");
    EXPECT_EQ(sd3.GetRaw(), 1234000);
    auto sd_neg = ShortDecimal<4>::FromString("-123.4567");
    EXPECT_EQ(sd_neg.GetRaw(), -1234567);
    auto sd_neg2 = ShortDecimal<4>::FromString("-123.45");
    EXPECT_EQ(sd_neg2.GetRaw(), -1234500);
    auto sd_neg3 = ShortDecimal<4>::FromString("-123.0567");
    EXPECT_EQ(sd_neg3.GetRaw(), -1230567);
}

TEST(ShortDecimalTest, FromStringInvalidDecimal) {
    EXPECT_THROW(ShortDecimal<4>::FromString("123.45678"), std::invalid_argument);
    EXPECT_THROW(ShortDecimal<4>::FromString("12a.45"), std::invalid_argument);
}

TEST(ShortDecimalTest, FromStringEdgeCases) {
    auto sd1 = ShortDecimal<4>::FromString("0.0000");
    EXPECT_EQ(sd1.GetRaw(), 0);
    auto sd2 = ShortDecimal<4>::FromString("922337203685477.5807");
    EXPECT_EQ(sd2.GetRaw(), INT64_MAX);
    auto sd3 = ShortDecimal<4>::FromString("-922337203685477.5807");
    EXPECT_EQ(sd3.GetRaw(), INT64_MIN+1);
}

TEST(ShortDecimalTest, ToString) {
    auto sd1 = ShortDecimal<4>::FromRaw(1234567);
    EXPECT_EQ(sd1.ToString(), "123.4567");
    auto sd2 = ShortDecimal<4>::FromRaw(1230067);
    EXPECT_EQ(sd2.ToString(), "123.0067");
    auto sd_neg1 = ShortDecimal<4>::FromRaw(-1234567);
    EXPECT_EQ(sd_neg1.ToString(), "-123.4567");
    auto sd_neg2 = ShortDecimal<4>::FromRaw(-1230567);
    EXPECT_EQ(sd_neg2.ToString(), "-123.0567");
}

TEST(ShortDecimalTest, Addition) {
    auto a = ShortDecimal<4>::FromRaw(10000);
    auto b = ShortDecimal<4>::FromRaw(20000);
    auto c = a + b;
    EXPECT_EQ(c.GetRaw(), 30000);
}

TEST(ShortDecimalTest, AdditionOverflow) {
    auto a = ShortDecimal<4>::FromRaw(INT64_MAX);
    auto b = ShortDecimal<4>::FromRaw(1);
    EXPECT_THROW(a + b, std::overflow_error);
}

TEST(ShortDecimalTest, Subtraction) {
    auto a = ShortDecimal<4>::FromRaw(30000);
    auto b = ShortDecimal<4>::FromRaw(20000);
    auto c = a - b;
    EXPECT_EQ(c.GetRaw(), 10000);
}

TEST(ShortDecimalTest, SubtractionUnderflow) {
    auto a = ShortDecimal<4>::FromRaw(INT64_MIN);
    auto b = ShortDecimal<4>::FromRaw(1);
    EXPECT_THROW(a - b, std::overflow_error);
}

TEST(ShortDecimalTest, CompoundOperators) {
    auto a = ShortDecimal<4>::FromRaw(10000);
    a += ShortDecimal<4>::FromRaw(20000);
    EXPECT_EQ(a.GetRaw(), 30000);
    a -= ShortDecimal<4>::FromRaw(10000);
    EXPECT_EQ(a.GetRaw(), 20000);
}

TEST(ShortDecimalTest, ComparisonOperators) {
    auto a = ShortDecimal<4>::FromRaw(10000);
    auto b = ShortDecimal<4>::FromRaw(20000);
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a <= b);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(b >= a);
}

// ---------------------- LongDecimal Tests (Precision=18) ----------------------

TEST(LongDecimalTest, FromIntegerValid) {
    auto ld1 = LongDecimal<18>::FromInteger(123);
    EXPECT_EQ(ld1.GetRaw().integer, 123);
    EXPECT_EQ(ld1.GetRaw().decimal, 0);
    auto ld2 = LongDecimal<18>::FromInteger(-123);
    EXPECT_EQ(ld2.GetRaw().integer, -123);
    EXPECT_EQ(ld2.GetRaw().decimal, 0);
}

TEST(LongDecimalTest, FromDoubleValid) {
    auto ld1 = LongDecimal<18>::FromDouble(123.456789012345678901);
    EXPECT_EQ(ld1.GetRaw().integer, 123);
    EXPECT_NEAR(ld1.GetRaw().decimal, 456789012345678901, 1e4);
    auto ld2 = LongDecimal<18>::FromDouble(-123.456789012345678901);
    EXPECT_EQ(ld2.GetRaw().integer, -123);
    EXPECT_NEAR(ld2.GetRaw().decimal, 456789012345678901, 1e4);
}

TEST(LongDecimalTest, FromDoubleOverflow) {
    double huge_value1 = 1e30;
    EXPECT_THROW(LongDecimal<18>::FromDouble(huge_value1), std::overflow_error);
    double huge_value2 = -1e30;
    EXPECT_THROW(LongDecimal<18>::FromDouble(huge_value2), std::overflow_error);
}

TEST(LongDecimalTest, FromStringValid) {
    auto ld1 = LongDecimal<18>::FromString("123456789012345678.123456789012345678");
    EXPECT_EQ(ld1.GetRaw().integer, 123456789012345678);
    EXPECT_EQ(ld1.GetRaw().decimal, 123456789012345678);
    auto ld2 = LongDecimal<18>::FromString("+123456789012345678.123456789012345678");
    EXPECT_EQ(ld2.GetRaw().integer, 123456789012345678);
    EXPECT_EQ(ld2.GetRaw().decimal, 123456789012345678);
    auto ld3 = LongDecimal<18>::FromString("+123456789012345678.000012345678901234");
    EXPECT_EQ(ld3.GetRaw().integer, 123456789012345678);
    EXPECT_EQ(ld3.GetRaw().decimal, 12345678901234);
    auto ld4 = LongDecimal<18>::FromString("-123456789012345678.123456789012345678");
    EXPECT_EQ(ld4.GetRaw().integer, -123456789012345678);
    EXPECT_EQ(ld4.GetRaw().decimal, 123456789012345678);
    auto ld5 = LongDecimal<18>::FromString("-123456789012345678.000012345678901234");
    EXPECT_EQ(ld5.GetRaw().integer, -123456789012345678);
    EXPECT_EQ(ld5.GetRaw().decimal, 12345678901234);
}

TEST(LongDecimalTest, FromStringInvalid) {
    EXPECT_THROW(LongDecimal<18>::FromString("123.1234567890123456789"), std::invalid_argument);
    EXPECT_THROW(LongDecimal<18>::FromString("12a.123456789012345678"), std::invalid_argument);
}

TEST(LongDecimalTest, ToString) {
    auto ld1 = LongDecimal<18>::FromRaw({123, 456789012345678901});
    EXPECT_EQ(ld1.ToString(), "123.456789012345678901");
    auto ld2 = LongDecimal<18>::FromRaw({-123, 456789012345678901});
    EXPECT_EQ(ld2.ToString(), "-123.456789012345678901");
    auto ld3 = LongDecimal<18>::FromRaw({123, 45678901234567});
    EXPECT_EQ(ld3.ToString(), "123.000045678901234567");
    auto ld4 = LongDecimal<18>::FromRaw({-123, 45678901234567});
    EXPECT_EQ(ld4.ToString(), "-123.000045678901234567");
}

TEST(LongDecimalTest, AdditionWithCarry) {
    auto a = LongDecimal<18>::FromRaw(0, PowerOfTen<18>::value - 1);
    auto b = LongDecimal<18>::FromRaw(0, 2);
    auto c = a + b;
    EXPECT_EQ(c.GetRaw().integer, 1);
    EXPECT_EQ(c.GetRaw().decimal, 1);
}

TEST(LongDecimalTest, AdditionOverflow) {
    auto a = LongDecimal<18>::FromRaw(INT64_MAX, 0);
    auto b = LongDecimal<18>::FromRaw(1, 0);
    EXPECT_THROW(a + b, std::overflow_error);
}

TEST(LongDecimalTest, SubtractionWithBorrow) {
    auto a = LongDecimal<18>::FromRaw(1, 0);
    auto b = LongDecimal<18>::FromRaw(0, 1);
    auto c = a - b;
    EXPECT_EQ(c.GetRaw().integer, 0);
    EXPECT_EQ(c.GetRaw().decimal, PowerOfTen<18>::value - 1);
}

TEST(LongDecimalTest, SubtractionUnderflow) {
    auto a = LongDecimal<18>::FromRaw(INT64_MIN, 0);
    auto b = LongDecimal<18>::FromRaw(1, 0);
    EXPECT_THROW(a - b, std::overflow_error);
}

TEST(LongDecimalTest, CompoundOperators) {
    auto a = LongDecimal<18>::FromRaw(1, 500000000000000000);
    a += LongDecimal<18>::FromRaw(2, 500000000000000000);
    EXPECT_EQ(a.GetRaw().integer, 4);
    EXPECT_EQ(a.GetRaw().decimal, 0);
    a -= LongDecimal<18>::FromRaw(1, 0);
    EXPECT_EQ(a.GetRaw().integer, 3);
    EXPECT_EQ(a.GetRaw().decimal, 0);
}

TEST(LongDecimalTest, ComparisonOperators) {
    auto a = LongDecimal<18>::FromRaw(1, 100000000000000000);
    auto b = LongDecimal<18>::FromRaw(1, 200000000000000000);
    auto c = LongDecimal<18>::FromRaw(2, 0);
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b < c);
    EXPECT_TRUE(a != b);
    EXPECT_TRUE(c > b);
}

// ---------------------- ShortDecimal<0> Tests ----------------------

TEST(ShortDecimal0Test, FromIntegerValid) {
    auto sd = ShortDecimal<0>::FromInteger(123);
    EXPECT_EQ(sd.GetRaw(), 123);
    auto sd_max = ShortDecimal<0>::FromInteger(INT64_MAX);
    EXPECT_EQ(sd_max.GetRaw(), INT64_MAX);
}

TEST(ShortDecimal0Test, FromDoubleValid) {
    auto sd1 = ShortDecimal<0>::FromDouble(123.0);
    EXPECT_EQ(sd1.GetRaw(), 123);
    auto sd2 = ShortDecimal<0>::FromDouble(123.999);
    EXPECT_EQ(sd2.GetRaw(), 123);
}

TEST(ShortDecimal0Test, FromDoubleOverflow) {
    double overflow = static_cast<double>(INT64_MAX) * 2;
    EXPECT_THROW(ShortDecimal<0>::FromDouble(overflow), std::overflow_error);
}

TEST(ShortDecimal0Test, FromStringValid) {
    auto sd1 = ShortDecimal<0>::FromString("123");
    EXPECT_EQ(sd1.GetRaw(), 123);
    auto sd2 = ShortDecimal<0>::FromString("-456");
    EXPECT_EQ(sd2.GetRaw(), -456);
}

TEST(ShortDecimal0Test, FromStringInvalid) {
    EXPECT_THROW(ShortDecimal<0>::FromString("123.4"), std::invalid_argument);
    EXPECT_THROW(ShortDecimal<0>::FromString("12a"), std::invalid_argument);
}

TEST(ShortDecimal0Test, ToString) {
    auto sd1 = ShortDecimal<0>::FromRaw(123);
    EXPECT_EQ(sd1.ToString(), "123");
    auto sd2 = ShortDecimal<0>::FromRaw(-456);
    EXPECT_EQ(sd2.ToString(), "-456");
}

TEST(ShortDecimal0Test, Addition) {
    auto a = ShortDecimal<0>::FromInteger(100);
    auto b = ShortDecimal<0>::FromInteger(200);
    auto c = a + b;
    EXPECT_EQ(c.GetRaw(), 300);
}

TEST(ShortDecimal0Test, AdditionOverflow) {
    auto a = ShortDecimal<0>::FromRaw(INT64_MAX);
    auto b = ShortDecimal<0>::FromRaw(1);
    EXPECT_THROW(a + b, std::overflow_error);
}

TEST(ShortDecimal0Test, Subtraction) {
    auto a = ShortDecimal<0>::FromRaw(500);
    auto b = ShortDecimal<0>::FromRaw(200);
    auto c = a - b;
    EXPECT_EQ(c.GetRaw(), 300);
}

TEST(ShortDecimal0Test, SubtractionUnderflow) {
    auto a = ShortDecimal<0>::FromRaw(INT64_MIN);
    auto b = ShortDecimal<0>::FromRaw(1);
    EXPECT_THROW(a - b, std::overflow_error);
}

TEST(ShortDecimal0Test, Comparison) {
    auto a = ShortDecimal<0>::FromRaw(100);
    auto b = ShortDecimal<0>::FromRaw(200);
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

// ---------------------- LongDecimal<0> Tests ----------------------

TEST(LongDecimal0Test, FromIntegerValid) {
    auto ld = LongDecimal<0>::FromInteger(123456);
    EXPECT_EQ(ld.GetRaw().integer, 123456);
    EXPECT_EQ(ld.GetRaw().decimal, 0);
}

TEST(LongDecimal0Test, FromDoubleValid) {
    auto ld1 = LongDecimal<0>::FromDouble(123.0);
    EXPECT_EQ(ld1.GetRaw().integer, 123);
    EXPECT_EQ(ld1.GetRaw().decimal, 0);
    auto ld2 = LongDecimal<0>::FromDouble(123.999);
    EXPECT_EQ(ld2.GetRaw().integer, 123);
    EXPECT_EQ(ld2.GetRaw().decimal, 0);
}

TEST(LongDecimal0Test, FromDoubleInvalidDecimal) {
    EXPECT_THROW(LongDecimal<0>::FromRaw(123, 1), std::overflow_error);
}

TEST(LongDecimal0Test, FromStringValid) {
    auto ld1 = LongDecimal<0>::FromString("123456789");
    EXPECT_EQ(ld1.GetRaw().integer, 123456789);
    EXPECT_EQ(ld1.GetRaw().decimal, 0);
}

TEST(LongDecimal0Test, FromStringInvalid) {
    EXPECT_THROW(LongDecimal<0>::FromString("123.0"), std::invalid_argument);
    EXPECT_THROW(LongDecimal<0>::FromString("12a"), std::invalid_argument);
}

TEST(LongDecimal0Test, ToString) {
    auto ld1 = LongDecimal<0>::FromRaw({123, 0});
    EXPECT_EQ(ld1.ToString(), "123");
    auto ld2 = LongDecimal<0>::FromRaw({-456, 0});
    EXPECT_EQ(ld2.ToString(), "-456");
}

TEST(LongDecimal0Test, Addition) {
    auto a = LongDecimal<0>::FromInteger(100);
    auto b = LongDecimal<0>::FromInteger(200);
    auto c = a + b;
    EXPECT_EQ(c.GetRaw().integer, 300);
    EXPECT_EQ(c.GetRaw().decimal, 0);
}

TEST(LongDecimal0Test, AdditionOverflow) {
    auto a = LongDecimal<0>::FromRaw({INT64_MAX, 0});
    auto b = LongDecimal<0>::FromRaw({1, 0});
    EXPECT_THROW(a + b, std::overflow_error);
}

TEST(LongDecimal0Test, Subtraction) {
    auto a = LongDecimal<0>::FromRaw({500, 0});
    auto b = LongDecimal<0>::FromRaw({200, 0});
    auto c = a - b;
    EXPECT_EQ(c.GetRaw().integer, 300);
    EXPECT_EQ(c.GetRaw().decimal, 0);
}

TEST(LongDecimal0Test, SubtractionUnderflow) {
    auto a = LongDecimal<0>::FromRaw({INT64_MIN, 0});
    auto b = LongDecimal<0>::FromRaw({1, 0});
    EXPECT_THROW(a - b, std::overflow_error);
}

TEST(LongDecimal0Test, Comparison) {
    auto a = LongDecimal<0>::FromRaw({100, 0});
    auto b = LongDecimal<0>::FromRaw({200, 0});
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}