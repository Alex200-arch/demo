#include <gtest/gtest.h>
#include "../decimal_base.hpp"

// ---------------------- Price/ShortQuantity Tests (Precision=4) ----------------------

TEST(PriceTest, FromIntegerValid) {
    auto sd1 = Price<4>::FromInteger(123);
    EXPECT_EQ(sd1.GetRaw(), 1230000);
    auto sd2 = Price<4>::FromInteger(-123);
    EXPECT_EQ(sd2.GetRaw(), -1230000);
}

TEST(PriceTest, FromIntegerOverflow) {
    int64_t max_int = INT64_MAX / 10000;
    EXPECT_NO_THROW(Price<4>::FromInteger(max_int));
    int64_t overflow_max_int = max_int + 1;
    EXPECT_THROW(Price<4>::FromInteger(overflow_max_int), std::overflow_error);
    int64_t min_int = INT64_MIN / 10000;
    EXPECT_NO_THROW(Price<4>::FromInteger(min_int));
    int64_t overflow_min_int = min_int -1;
    EXPECT_THROW(Price<4>::FromInteger(overflow_min_int), std::overflow_error);
}

TEST(PriceTest, FromDoubleValid) {
    auto sd1 = Price<4>::FromDouble(123.4567);
    EXPECT_EQ(sd1.GetRaw(), 1234567);
    auto sd2= Price<4>::FromDouble(-123.4567);
    EXPECT_EQ(sd2.GetRaw(), -1234567);
}

TEST(PriceTest, FromDoubleOverflow) {
    double overflow_double1 = static_cast<double>(INT64_MAX) + 1e10;
    EXPECT_THROW(Price<4>::FromDouble(overflow_double1), std::overflow_error);
    double overflow_double2 = static_cast<double>(INT64_MIN) - 1e10;
    EXPECT_THROW(Price<4>::FromDouble(overflow_double2), std::overflow_error);
}

TEST(PriceTest, FromStringValid) {
    auto sd1 = Price<4>::FromString("123.4567");
    EXPECT_EQ(sd1.GetRaw(), 1234567);
    auto sd2 = Price<4>::FromString("+123.0567");
    EXPECT_EQ(sd2.GetRaw(), 1230567);
    auto sd3 = Price<4>::FromString("+123.4");
    EXPECT_EQ(sd3.GetRaw(), 1234000);
    auto sd_neg = Price<4>::FromString("-123.4567");
    EXPECT_EQ(sd_neg.GetRaw(), -1234567);
    auto sd_neg2 = Price<4>::FromString("-123.45");
    EXPECT_EQ(sd_neg2.GetRaw(), -1234500);
    auto sd_neg3 = Price<4>::FromString("-123.0567");
    EXPECT_EQ(sd_neg3.GetRaw(), -1230567);
}

TEST(PriceTest, FromStringInvalidDecimal) {
    EXPECT_THROW(Price<4>::FromString("123.45678"), std::invalid_argument);
    EXPECT_THROW(Price<4>::FromString("12a.45"), std::invalid_argument);
}

TEST(PriceTest, FromStringEdgeCases) {
    auto sd1 = Price<4>::FromString("0.0000");
    EXPECT_EQ(sd1.GetRaw(), 0);
    auto sd2 = Price<4>::FromString("922337203685477.5807");
    EXPECT_EQ(sd2.GetRaw(), INT64_MAX);
    auto sd3 = Price<4>::FromString("-922337203685477.5807");
    EXPECT_EQ(sd3.GetRaw(), INT64_MIN+1);
}

TEST(PriceTest, ToString) {
    auto sd1 = Price<4>::FromRaw(1234567);
    EXPECT_EQ(sd1.ToString(), "123.4567");
    auto sd2 = Price<4>::FromRaw(1230067);
    EXPECT_EQ(sd2.ToString(), "123.0067");
    auto sd_neg1 = Price<4>::FromRaw(-1234567);
    EXPECT_EQ(sd_neg1.ToString(), "-123.4567");
    auto sd_neg2 = Price<4>::FromRaw(-1230567);
    EXPECT_EQ(sd_neg2.ToString(), "-123.0567");
}

TEST(PriceTest, Addition) {
    auto a = Price<4>::FromRaw(10000);
    auto b = Price<4>::FromRaw(20000);
    auto c = a + b;
    EXPECT_EQ(c.GetRaw(), 30000);
}

TEST(PriceTest, AdditionOverflow) {
    auto a = Price<4>::FromRaw(INT64_MAX);
    auto b = Price<4>::FromRaw(1);
    EXPECT_THROW(a + b, std::overflow_error);
}

TEST(PriceTest, Subtraction) {
    auto a = Price<4>::FromRaw(30000);
    auto b = Price<4>::FromRaw(20000);
    auto c = a - b;
    EXPECT_EQ(c.GetRaw(), 10000);
}

TEST(PriceTest, SubtractionUnderflow) {
    auto a = Price<4>::FromRaw(INT64_MIN);
    auto b = Price<4>::FromRaw(1);
    EXPECT_THROW(a - b, std::overflow_error);
}

TEST(PriceTest, CompoundOperators) {
    auto a = Price<4>::FromRaw(10000);
    a += Price<4>::FromRaw(20000);
    EXPECT_EQ(a.GetRaw(), 30000);
    a -= Price<4>::FromRaw(10000);
    EXPECT_EQ(a.GetRaw(), 20000);
}

TEST(PriceTest, ComparisonOperators) {
    auto a = Price<4>::FromRaw(10000);
    auto b = Price<4>::FromRaw(20000);
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a <= b);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(b >= a);
}

// ---------------------- LongQuantity Tests (Precision=18) ----------------------

TEST(LongQuantityTest, FromIntegerValid) {
    auto ld1 = LongQuantity<18>::FromInteger(123);
    EXPECT_EQ(ld1.GetRaw().integer, 123);
    EXPECT_EQ(ld1.GetRaw().decimal, 0);
    auto ld2 = LongQuantity<18>::FromInteger(-123);
    EXPECT_EQ(ld2.GetRaw().integer, -123);
    EXPECT_EQ(ld2.GetRaw().decimal, 0);
}

TEST(LongQuantityTest, FromDoubleValid) {
    auto ld1 = LongQuantity<18>::FromDouble(123.456789012345678901);
    EXPECT_EQ(ld1.GetRaw().integer, 123);
    EXPECT_NEAR(ld1.GetRaw().decimal, 456789012345678901, 1e4);
    auto ld2 = LongQuantity<18>::FromDouble(-123.456789012345678901);
    EXPECT_EQ(ld2.GetRaw().integer, -123);
    EXPECT_NEAR(ld2.GetRaw().decimal, -456789012345678901, 1e4);
}

TEST(LongQuantityTest, FromDoubleOverflow) {
    double huge_value1 = 1e30;
    EXPECT_THROW(LongQuantity<18>::FromDouble(huge_value1), std::overflow_error);
    double huge_value2 = -1e30;
    EXPECT_THROW(LongQuantity<18>::FromDouble(huge_value2), std::overflow_error);
}

TEST(LongQuantityTest, FromStringValid) {
    auto ld1 = LongQuantity<18>::FromString("123456789012345678.123456789012345678");
    EXPECT_EQ(ld1.GetRaw().integer, 123456789012345678);
    EXPECT_EQ(ld1.GetRaw().decimal, 123456789012345678);
    auto ld2 = LongQuantity<18>::FromString("+123456789012345678.123456789012345678");
    EXPECT_EQ(ld2.GetRaw().integer, 123456789012345678);
    EXPECT_EQ(ld2.GetRaw().decimal, 123456789012345678);
    auto ld3 = LongQuantity<18>::FromString("+123456789012345678.000012345678901234");
    EXPECT_EQ(ld3.GetRaw().integer, 123456789012345678);
    EXPECT_EQ(ld3.GetRaw().decimal, 12345678901234);
    auto ld4 = LongQuantity<18>::FromString("-123456789012345678.123456789012345678");
    EXPECT_EQ(ld4.GetRaw().integer, -123456789012345678);
    EXPECT_EQ(ld4.GetRaw().decimal, -123456789012345678);
    auto ld5 = LongQuantity<18>::FromString("-123456789012345678.000012345678901234");
    EXPECT_EQ(ld5.GetRaw().integer, -123456789012345678);
    EXPECT_EQ(ld5.GetRaw().decimal, -12345678901234);
}

TEST(LongQuantityTest, FromStringInvalid) {
    EXPECT_THROW(LongQuantity<18>::FromString("123.1234567890123456789"), std::invalid_argument);
    EXPECT_THROW(LongQuantity<18>::FromString("12a.123456789012345678"), std::invalid_argument);
}

TEST(LongQuantityTest, ToString) {
    auto ld1 = LongQuantity<18>::FromRaw({123, 456789012345678901});
    EXPECT_EQ(ld1.ToString(), "123.456789012345678901");
    auto ld2 = LongQuantity<18>::FromRaw({-123, -456789012345678901});
    EXPECT_EQ(ld2.ToString(), "-123.456789012345678901");
    auto ld3 = LongQuantity<18>::FromRaw({123, 45678901234567});
    EXPECT_EQ(ld3.ToString(), "123.000045678901234567");
    auto ld4 = LongQuantity<18>::FromRaw({-123, -45678901234567});
    EXPECT_EQ(ld4.ToString(), "-123.000045678901234567");
    auto ld5 = LongQuantity<18>::FromRaw({0, 45678901234567});
    EXPECT_EQ(ld5.ToString(), "0.000045678901234567");
    auto ld6 = LongQuantity<18>::FromRaw({0, -45678901234567});
    EXPECT_EQ(ld6.ToString(), "-0.000045678901234567");
}

TEST(LongQuantityTest, AdditionWithCarry) {
    auto a = LongQuantity<18>::FromRaw(0, PowerOfTen<18>::value - 1);
    auto b = LongQuantity<18>::FromRaw(0, 2);
    auto c = a + b;
    EXPECT_EQ(c.GetRaw().integer, 1);
    EXPECT_EQ(c.GetRaw().decimal, 1);

    auto a1 = LongQuantity<18>::FromRaw(-10, -9);
    auto b1 = LongQuantity<18>::FromRaw(4, 200);
    auto c1 = a1 + b1;      //-10.000000000000000009 + 4.000000000000000200
    EXPECT_EQ(c1.GetRaw().integer, -5);
    EXPECT_EQ(c1.GetRaw().decimal, -999999999999999809);
}

TEST(LongQuantityTest, AdditionOverflow) {
    auto a = LongQuantity<18>::FromRaw(INT64_MAX, 0);
    auto b = LongQuantity<18>::FromRaw(1, 0);
    EXPECT_THROW(a + b, std::overflow_error);
}

TEST(LongQuantityTest, SubtractionWithBorrow) {
    auto a = LongQuantity<18>::FromRaw(1, 0);
    auto b = LongQuantity<18>::FromRaw(0, 1);
    auto c = a - b;
    EXPECT_EQ(c.GetRaw().integer, 0);
    EXPECT_EQ(c.GetRaw().decimal, PowerOfTen<18>::value - 1);

    auto a1 = LongQuantity<18>::FromRaw(4, 9);
    auto b1 = LongQuantity<18>::FromRaw(-100, -1);
    auto c1 = a1 - b1;           // 4.000000000000000009 - (-100.000000000000000001)
                                 // 104.000000000000000010
    EXPECT_EQ(c1.GetRaw().integer, 104);
    EXPECT_EQ(c1.GetRaw().decimal, 10);
}

TEST(LongQuantityTest, SubtractionUnderflow) {
    auto a = LongQuantity<18>::FromRaw(INT64_MIN, 0);
    auto b = LongQuantity<18>::FromRaw(1, 0);
    EXPECT_THROW(a - b, std::overflow_error);
}

TEST(LongQuantityTest, CompoundOperators) {
    auto a = LongQuantity<18>::FromRaw(1, 500000000000000000);
    a += LongQuantity<18>::FromRaw(2, 500000000000000000);
    EXPECT_EQ(a.GetRaw().integer, 4);
    EXPECT_EQ(a.GetRaw().decimal, 0);
    a -= LongQuantity<18>::FromRaw(1, 0);
    EXPECT_EQ(a.GetRaw().integer, 3);
    EXPECT_EQ(a.GetRaw().decimal, 0);
}

TEST(LongQuantityTest, ComparisonOperators) {
    auto a = LongQuantity<18>::FromRaw(1, 100000000000000000);
    auto b = LongQuantity<18>::FromRaw(1, 200000000000000000);
    auto c = LongQuantity<18>::FromRaw(2, 0);
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b < c);
    EXPECT_TRUE(a != b);
    EXPECT_TRUE(c > b);
}

// ---------------------- Price/ShortQuantity<0> Tests ----------------------

TEST(PriceTest0Test, FromIntegerValid) {
    auto sd = Price<0>::FromInteger(123);
    EXPECT_EQ(sd.GetRaw(), 123);
    auto sd_max = Price<0>::FromInteger(INT64_MAX);
    EXPECT_EQ(sd_max.GetRaw(), INT64_MAX);
}

TEST(PriceTest0Test, FromDoubleValid) {
    auto sd1 = Price<0>::FromDouble(123.0);
    EXPECT_EQ(sd1.GetRaw(), 123);
    auto sd2 = Price<0>::FromDouble(123.999);
    EXPECT_EQ(sd2.GetRaw(), 123);
}

TEST(PriceTest0Test, FromDoubleOverflow) {
    double overflow = static_cast<double>(INT64_MAX) * 2;
    EXPECT_THROW(Price<0>::FromDouble(overflow), std::overflow_error);
}

TEST(PriceTest0Test, FromStringValid) {
    auto sd1 = Price<0>::FromString("123");
    EXPECT_EQ(sd1.GetRaw(), 123);
    auto sd2 = Price<0>::FromString("-456");
    EXPECT_EQ(sd2.GetRaw(), -456);
}

TEST(PriceTest0Test, FromStringInvalid) {
    EXPECT_THROW(Price<0>::FromString("123.4"), std::invalid_argument);
    EXPECT_THROW(Price<0>::FromString("12a"), std::invalid_argument);
}

TEST(PriceTest0Test, ToString) {
    auto sd1 = Price<0>::FromRaw(123);
    EXPECT_EQ(sd1.ToString(), "123");
    auto sd2 = Price<0>::FromRaw(-456);
    EXPECT_EQ(sd2.ToString(), "-456");
}

TEST(PriceTest0Test, Addition) {
    auto a = Price<0>::FromInteger(100);
    auto b = Price<0>::FromInteger(200);
    auto c = a + b;
    EXPECT_EQ(c.GetRaw(), 300);
}

TEST(PriceTest0Test, AdditionOverflow) {
    auto a = Price<0>::FromRaw(INT64_MAX);
    auto b = Price<0>::FromRaw(1);
    EXPECT_THROW(a + b, std::overflow_error);
}

TEST(PriceTest0Test, Subtraction) {
    auto a = Price<0>::FromRaw(500);
    auto b = Price<0>::FromRaw(200);
    auto c = a - b;
    EXPECT_EQ(c.GetRaw(), 300);
}

TEST(PriceTest0Test, SubtractionUnderflow) {
    auto a = Price<0>::FromRaw(INT64_MIN);
    auto b = Price<0>::FromRaw(1);
    EXPECT_THROW(a - b, std::overflow_error);
}

TEST(PriceTest0Test, Comparison) {
    auto a = Price<0>::FromRaw(100);
    auto b = Price<0>::FromRaw(200);
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

// ---------------------- LongQuantity<0> Tests ----------------------

TEST(LongQuantity0Test, FromIntegerValid) {
    auto ld = LongQuantity<0>::FromInteger(123456);
    EXPECT_EQ(ld.GetRaw().integer, 123456);
    EXPECT_EQ(ld.GetRaw().decimal, 0);
}

TEST(LongQuantity0Test, FromDoubleValid) {
    auto ld1 = LongQuantity<0>::FromDouble(123.0);
    EXPECT_EQ(ld1.GetRaw().integer, 123);
    EXPECT_EQ(ld1.GetRaw().decimal, 0);
    auto ld2 = LongQuantity<0>::FromDouble(123.999);
    EXPECT_EQ(ld2.GetRaw().integer, 123);
    EXPECT_EQ(ld2.GetRaw().decimal, 0);
}

TEST(LongQuantity0Test, FromDoubleInvalidDecimal) {
    EXPECT_THROW(LongQuantity<0>::FromRaw(123, 1), std::overflow_error);
}

TEST(LongQuantity0Test, FromStringValid) {
    auto ld1 = LongQuantity<0>::FromString("123456789");
    EXPECT_EQ(ld1.GetRaw().integer, 123456789);
    EXPECT_EQ(ld1.GetRaw().decimal, 0);
}

TEST(LongQuantity0Test, FromStringInvalid) {
    EXPECT_THROW(LongQuantity<0>::FromString("123.0"), std::invalid_argument);
    EXPECT_THROW(LongQuantity<0>::FromString("12a"), std::invalid_argument);
}

TEST(LongQuantity0Test, ToString) {
    auto ld1 = LongQuantity<0>::FromRaw({123, 0});
    EXPECT_EQ(ld1.ToString(), "123");
    auto ld2 = LongQuantity<0>::FromRaw({-456, 0});
    EXPECT_EQ(ld2.ToString(), "-456");
}

TEST(LongQuantity0Test, Addition) {
    auto a = LongQuantity<0>::FromInteger(100);
    auto b = LongQuantity<0>::FromInteger(200);
    auto c = a + b;
    EXPECT_EQ(c.GetRaw().integer, 300);
    EXPECT_EQ(c.GetRaw().decimal, 0);
}

TEST(LongQuantity0Test, AdditionOverflow) {
    auto a = LongQuantity<0>::FromRaw({INT64_MAX, 0});
    auto b = LongQuantity<0>::FromRaw({1, 0});
    EXPECT_THROW(a + b, std::overflow_error);
}

TEST(LongQuantity0Test, Subtraction) {
    auto a = LongQuantity<0>::FromRaw({500, 0});
    auto b = LongQuantity<0>::FromRaw({200, 0});
    auto c = a - b;
    EXPECT_EQ(c.GetRaw().integer, 300);
    EXPECT_EQ(c.GetRaw().decimal, 0);
}

TEST(LongQuantity0Test, SubtractionUnderflow) {
    auto a = LongQuantity<0>::FromRaw({INT64_MIN, 0});
    auto b = LongQuantity<0>::FromRaw({1, 0});
    EXPECT_THROW(a - b, std::overflow_error);
}

TEST(LongQuantity0Test, Comparison) {
    auto a = LongQuantity<0>::FromRaw({100, 0});
    auto b = LongQuantity<0>::FromRaw({200, 0});
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

// ---------------------- Price/ShortQuantity ----------------------
TEST(PriceTest, MultiplyInteger) {
    auto a = Price<4>::FromDouble(12.34);
    auto b = a * 3;
    EXPECT_EQ(b.ToDouble(), 37.02);
    auto c = 3 * a;
    EXPECT_EQ(c.ToDouble(), 37.02);
}

TEST(PriceTest, MultiplyIntegerOverflow) {
    auto a = Price<0>::FromRaw(INT64_MAX);
    EXPECT_THROW(a * 2, std::overflow_error);
}

// ---------------------- LongQuantity ----------------------
TEST(LongQuantityTest, MultiplyInteger) {
    auto a = LongQuantity<2>::FromRaw({12, 34});
    auto b = a * 3;
    EXPECT_EQ(b.ToString(), "37.02");
    auto c = 3 * a;
    EXPECT_EQ(c.ToString(), "37.02");
}

TEST(LongQuantityTest, MultiplyNegative) {
    auto a = LongQuantity<2>::FromRaw({-5, -25}); 
    auto b = a * 4;
    EXPECT_EQ(b.ToString(), "-21.00");
}

TEST(LongQuantityTest, PrecisionZeroMultiply) {
    auto a = LongQuantity<0>::FromInteger(100);
    auto b = a * 3;
    EXPECT_EQ(b.ToString(), "300");
}

TEST(LongQuantityTest, MultiplyCarryOverflow) {
    auto a = LongQuantity<2>::FromRaw({INT64_MAX, 99});
    EXPECT_THROW(a * 2, std::overflow_error);
}

TEST(LongQuantityTest, MultiplyDecimalOverflow) {
    auto a = LongQuantity<2>::FromRaw({0, 50});
    auto b = a * 300;
    EXPECT_EQ(b.ToString(), "150.00");
}

// ---------------------- Price Multiplication ----------------------
TEST(PriceTest, MultiplyUpRounding) {
    Price<2> p = Price<2>::FromRaw(123);  // 1.23
    ShortQuantity<3> q = ShortQuantity<3>::FromRaw(4567);  // 4.567
    auto result = PxQ(p, q, RoundModelUP{});
    EXPECT_EQ(result.GetRaw(), 562);  // 1.23 * 4.567 = 5.61741 → UP → 5.62
}

TEST(PriceTest, MultiplyUpRoundingNegative) {
    Price<2> p = Price<2>::FromRaw(-123); // -1.23
    ShortQuantity<3> q = ShortQuantity<3>::FromRaw(4567); // 4.567
    auto result = PxQ(p, q, RoundModelUP{});
    EXPECT_EQ(result.GetRaw(), -562); // -1.23 * 4.567 = -5.61741 → UP → -5.62
}

TEST(PriceTest, MultiplyNearRoundingHalf) {
    Price<2> p = Price<2>::FromRaw(123); // 1.23
    ShortQuantity<3> q = ShortQuantity<3>::FromRaw(2500); // 2.500
    auto result = PxQ(p, q, RoundModelNEAR{});
    EXPECT_EQ(result.GetRaw(), 308); // 1.23 * 2.5 = 3.075 → NEAR → 3.08
}

TEST(PriceTest, MultiplyNearRoundingLessThanHalf) {
    Price<2> p = Price<2>::FromRaw(123); // 1.23
    ShortQuantity<3> q = ShortQuantity<3>::FromRaw(2499); // 2.499
    auto result = PxQ(p, q, RoundModelNEAR{});
    EXPECT_EQ(result.GetRaw(), 307); // 1.23 *2.499 = 3.07377 → NEAR → 3.07
    Price<2> p2 = Price<2>::FromRaw(-123); // -1.23
    ShortQuantity<3> q2 = ShortQuantity<3>::FromRaw(2499); // 2.499
    auto result2 = PxQ(p2, q2, RoundModelNEAR{});
    EXPECT_EQ(result2.GetRaw(), -307); // -1.23 *2.499 = -3.07377 → NEAR → -3.07
}

TEST(PriceTest, MultiplyDownRounding) {
    Price<2> p = Price<2>::FromRaw(123);
    ShortQuantity<3> q = ShortQuantity<3>::FromRaw(4567);
    auto result = PxQ(p, q, RoundModelDOWN{});
    EXPECT_EQ(result.GetRaw(), 561); // 1.23 * 4.567 = 5.61741 → DOWN → 5.61
}

TEST(PriceTest, MultiplySQOverflow) {
    Price<2> p = Price<2>::FromRaw(INT64_MAX);
    ShortQuantity<0> q = ShortQuantity<0>::FromRaw(2); // Integer 2
    EXPECT_THROW(PxQ(p, q, RoundModelDOWN{}), std::overflow_error);
}

TEST(LongDecimalPxQTest, RoundUpPositive) {
    Price<2> px = Price<2>::FromRaw(122); // 1.22
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({5, 123}); // 5.123
    auto result = PxQ(px, qty, RoundModelUP{});
    EXPECT_EQ(result.GetRaw(), 625); // 1.22 * 5.123 = 6.25006 → UP 6.25
}

TEST(LongDecimalPxQTest, RoundUpNegative) {
    Price<2> px = Price<2>::FromRaw(-178); // -1.78
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({5, 123}); // 5.123
    auto result = PxQ(px, qty, RoundModelUP{});
    EXPECT_EQ(result.GetRaw(), -912); // -1.78 * 5.123 = -9.11894 → UP -9.12
}

TEST(LongDecimalPxQTest, RoundDownPositive) {
    Price<2> px = Price<2>::FromRaw(153); // 1.53
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({3, 578}); // 3.578
    auto result = PxQ(px, qty, RoundModelDOWN{});
    EXPECT_EQ(result.GetRaw(), 547); // 1.53 * 3.578 = 5.47434 → DOWN 5.47
}

TEST(LongDecimalPxQTest, RoundDownNegative) {
    Price<2> px = Price<2>::FromRaw(-153); // -1.53
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({3, 578}); // 3.578
    auto result = PxQ(px, qty, RoundModelDOWN{});
    EXPECT_EQ(result.GetRaw(), -547); // -1.53 * 3.578 = -5.47434 → DOWN -5.47
}

TEST(LongDecimalPxQTest, RoundNearHalfUp) {
    Price<2> px = Price<2>::FromRaw(100); // 1.00
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({2, 500}); // 2.500
    auto result = PxQ(px, qty, RoundModelNEAR{});
    EXPECT_EQ(result.GetRaw(), 250); // 1.00 * 2.500 = 2.50000 → NEAR 2.50
}

TEST(LongDecimalPxQTest, RoundNearHalfDown) {
    Price<2> px = Price<2>::FromRaw(100); // 1.00
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({2, 499}); // 2.499
    auto result = PxQ(px, qty, RoundModelNEAR{});
    EXPECT_EQ(result.GetRaw(), 250); // 1.00 * 2.499 = 2.49900 → NEAR 2.50
}

TEST(LongDecimalPxQTest, OverflowCheck) {
    Price<2> px = Price<2>::FromRaw(INT64_MAX);
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({10, 0}); // 10.000
    EXPECT_THROW(PxQ(px, qty, RoundModelUP{}), std::overflow_error);
}
