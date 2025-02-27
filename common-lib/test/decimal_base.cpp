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
    auto b = a.MulInt(3);
    EXPECT_EQ(b.ToDouble(), 37.02);
}

TEST(PriceTest, MultiplyIntegerOverflow) {
    auto a = Price<0>::FromRaw(INT64_MAX);
    EXPECT_THROW(a.MulInt(2), std::overflow_error);
}

// ---------------------- LongQuantity ----------------------
TEST(LongQuantityTest, MultiplyInteger) {
    auto a = LongQuantity<2>::FromRaw({12, 34});
    auto b = a.MulInt(3);
    EXPECT_EQ(b.ToString(), "37.02");
}

TEST(LongQuantityTest, MultiplyNegative) {
    auto a = LongQuantity<2>::FromRaw({-5, -25}); 
    auto b = a.MulInt(4);
    EXPECT_EQ(b.ToString(), "-21.00");
}

TEST(LongQuantityTest, PrecisionZeroMultiply) {
    auto a = LongQuantity<0>::FromInteger(100);
    auto b = a.MulInt(3);
    EXPECT_EQ(b.ToString(), "300");
}

TEST(LongQuantityTest, MultiplyCarryOverflow) {
    auto a = LongQuantity<2>::FromRaw({INT64_MAX, 99});
    EXPECT_THROW(a.MulInt(2);, std::overflow_error);
}

TEST(LongQuantityTest, MultiplyDecimalOverflow) {
    auto a = LongQuantity<2>::FromRaw({0, 50});
    auto b = a.MulInt(300);;
    EXPECT_EQ(b.ToString(), "150.00");
}

// ---------------------- Price Multiplication ----------------------
TEST(PriceTest, MultiplyUpRounding) {
    Price<2> p = Price<2>::FromRaw(123);  // 1.23
    ShortQuantity<3> q = ShortQuantity<3>::FromRaw(4567);  // 4.567
    auto result = p.MulSQty(q, RoundModel::Up());
    EXPECT_EQ(result.GetRaw(), 562);  // 1.23 * 4.567 = 5.61741 -> UP -> 5.62
}

TEST(PriceTest, MultiplyUpRoundingNegative) {
    Price<2> p = Price<2>::FromRaw(-123); // -1.23
    ShortQuantity<3> q = ShortQuantity<3>::FromRaw(4567); // 4.567
    auto result = p.MulSQty(q, RoundModel::Up());
    EXPECT_EQ(result.GetRaw(), -562); // -1.23 * 4.567 = -5.61741 -> UP -> -5.62
}

TEST(PriceTest, MultiplyNearRoundingHalf) {
    Price<2> p = Price<2>::FromRaw(123); // 1.23
    ShortQuantity<3> q = ShortQuantity<3>::FromRaw(2500); // 2.500
    auto result = p.MulSQty(q, RoundModel::Near());
    EXPECT_EQ(result.GetRaw(), 308); // 1.23 * 2.5 = 3.075 -> NEAR -> 3.08
}

TEST(PriceTest, MultiplyNearRoundingLessThanHalf) {
    Price<2> p = Price<2>::FromRaw(123); // 1.23
    ShortQuantity<3> q = ShortQuantity<3>::FromRaw(2499); // 2.499
    auto result = p.MulSQty(q, RoundModel::Near());
    EXPECT_EQ(result.GetRaw(), 307); // 1.23 *2.499 = 3.07377 -> NEAR -> 3.07
    Price<2> p2 = Price<2>::FromRaw(-123); // -1.23
    ShortQuantity<3> q2 = ShortQuantity<3>::FromRaw(2499); // 2.499
    auto result2 = p2.MulSQty(q2, RoundModel::Near());
    EXPECT_EQ(result2.GetRaw(), -307); // -1.23 *2.499 = -3.07377 -> NEAR -> -3.07
}

TEST(PriceTest, MultiplyDownRounding) {
    Price<2> p = Price<2>::FromRaw(123);
    ShortQuantity<3> q = ShortQuantity<3>::FromRaw(4567);
    auto result = p.MulSQty(q, RoundModel::Down());
    EXPECT_EQ(result.GetRaw(), 561); // 1.23 * 4.567 = 5.61741 -> DOWN -> 5.61
}

TEST(PriceTest, MultiplySQOverflow) {
    Price<2> p = Price<2>::FromRaw(INT64_MAX);
    ShortQuantity<0> q = ShortQuantity<0>::FromRaw(2); // Integer 2
    EXPECT_THROW(p.MulSQty(q, RoundModel::Down()), std::overflow_error);
}

TEST(LongQuantityPxQTest, RoundUpPositive) {
    Price<2> px = Price<2>::FromRaw(122); // 1.22
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({5, 123}); // 5.123
    auto result = px.MulLQty(qty, RoundModel::Up());
    EXPECT_EQ(result.GetRaw(), 625); // 1.22 * 5.123 = 6.25006 -> UP 6.25
}

TEST(LongQuantityPxQTest, RoundUpNegative) {
    Price<2> px = Price<2>::FromRaw(-178); // -1.78
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({5, 123}); // 5.123
    auto result = px.MulLQty(qty, RoundModel::Up());
    EXPECT_EQ(result.GetRaw(), -912); // -1.78 * 5.123 = -9.11894 -> UP -9.12
}

TEST(LongQuantityPxQTest, RoundDownPositive) {
    Price<2> px = Price<2>::FromRaw(153); // 1.53
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({3, 578}); // 3.578
    auto result = px.MulLQty(qty, RoundModel::Down());
    EXPECT_EQ(result.GetRaw(), 547); // 1.53 * 3.578 = 5.47434 -> DOWN 5.47
}

TEST(LongQuantityPxQTest, RoundDownNegative) {
    Price<2> px = Price<2>::FromRaw(-153); // -1.53
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({3, 578}); // 3.578
    auto result = px.MulLQty(qty, RoundModel::Down());
    EXPECT_EQ(result.GetRaw(), -547); // -1.53 * 3.578 = -5.47434 -> DOWN -5.47
}

TEST(LongQuantityPxQTest, RoundNearHalfUp) {
    Price<2> px = Price<2>::FromRaw(100); // 1.00
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({2, 500}); // 2.500
    auto result = px.MulLQty(qty, RoundModel::Near());
    EXPECT_EQ(result.GetRaw(), 250); // 1.00 * 2.500 = 2.50000 -> NEAR 2.50
}

TEST(LongQuantityPxQTest, RoundNearHalfDown) {
    Price<2> px = Price<2>::FromRaw(100); // 1.00
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({2, 499}); // 2.499
    auto result = px.MulLQty(qty, RoundModel::Near());
    EXPECT_EQ(result.GetRaw(), 250); // 1.00 * 2.499 = 2.49900 -> NEAR 2.50
}

TEST(LongQuantityPxQTest, OverflowCheck) {
    Price<2> px = Price<2>::FromRaw(INT64_MAX);
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({10, 0}); // 10.000
    EXPECT_THROW(px.MulLQty(qty, RoundModel::Up()), std::overflow_error);
}

// ---------------------- Price/ShortQuantity Division ----------------------
TEST(PriceTest, DivisionWithRounding) {
    // 5.0000 / 3 = 1.66666666666666
    ShortQuantity<4> qty1 = ShortQuantity<4>::FromRaw(50000);
    auto result1 = qty1.DivInt(3, RoundModel::Up());
    EXPECT_EQ(result1.GetRaw(), 16667);

    ShortQuantity<4> qty2 = ShortQuantity<4>::FromRaw(-50000);
    auto result2 = qty2.DivInt(3, RoundModel::Up());
    EXPECT_EQ(result2.GetRaw(), -16667);
    
    ShortQuantity<4> qty3 = ShortQuantity<4>::FromRaw(50000);
    auto result3 = qty3.DivInt(-3, RoundModel::Up());
    EXPECT_EQ(result3.GetRaw(), -16667);
    
    ShortQuantity<4> qty4 = ShortQuantity<4>::FromRaw(-50000);
    auto result4 = qty4.DivInt(-3, RoundModel::Up());
    EXPECT_EQ(result4.GetRaw(), 16667);
    
    ShortQuantity<4> qty5 = ShortQuantity<4>::FromRaw(50000);
    auto result5 = qty5.DivInt(-3, RoundModel::Down());
    EXPECT_EQ(result5.GetRaw(), -16666);
    
    ShortQuantity<4> qty6 = ShortQuantity<4>::FromRaw(-50000);
    auto result6 = qty6.DivInt(3, RoundModel::Near());
    EXPECT_EQ(result6.GetRaw(), -16667);
    
    // 5.0000 / 7 = 0.714285714285714285
    ShortQuantity<4> qty7 = ShortQuantity<4>::FromRaw(-50000);
    auto result7 = qty7.DivInt(7, RoundModel::Near());
    EXPECT_EQ(result7.GetRaw(), -7143);
    
    // 5.0000 / 100000 = 0.00005
    ShortQuantity<4> qty8 = ShortQuantity<4>::FromRaw(-50000);
    auto result8 = qty8.DivInt(100000, RoundModel::Near());
    EXPECT_EQ(result8.GetRaw(), -1);
    
    // 5.0000 / 1000000 = 0.000005
    ShortQuantity<4> qty9 = ShortQuantity<4>::FromRaw(-50000);
    auto result9 = qty9.DivInt(1000000, RoundModel::Near());
    EXPECT_EQ(result9.GetRaw(), 0);
}

TEST(PriceTest, DivisionOverflow) {
    ShortQuantity<4> max_qty = ShortQuantity<4>::FromRaw(INT64_MAX);
    EXPECT_THROW(max_qty.DivInt(0, RoundModel::Up()), std::invalid_argument);
}

// ---------------------- LongQuantity Division ----------------------
TEST(LongQuantityTest, DivisionWithRounding) {
    LongQuantity<3> qty = LongQuantity<3>::FromRaw({5, 123}); // 5.123
    auto result = qty.DivInt(2, RoundModel::Up());
    EXPECT_EQ(result.GetRaw().integer, 2);
    EXPECT_EQ(result.GetRaw().decimal, 562); // 5.123 / 2 = 2.5615 -> 2.562

    LongQuantity<3> neg_qty = LongQuantity<3>::FromRaw({-5, -123}); // -5.123
    auto neg_result = neg_qty.DivInt(2, RoundModel::Up());
    EXPECT_EQ(neg_result.GetRaw().integer, -2);
    EXPECT_EQ(neg_result.GetRaw().decimal, -562); // -5.123 /2 = -2.5615 -> -2.562

    LongQuantity<3> exact_qty = LongQuantity<3>::FromRaw({10, 500}); // 10.500
    auto exact_result = exact_qty.DivInt(2, RoundModel::Up());
    EXPECT_EQ(exact_result.GetRaw().integer, 5);
    EXPECT_EQ(exact_result.GetRaw().decimal, 250); // 10.500/2=5.250

    LongQuantity<6> qty1 = LongQuantity<6>::FromRaw({5, 123}); // 5.000123
    auto result1 = qty1.DivInt(2, RoundModel::Up());
    EXPECT_EQ(result1.GetRaw().integer, 2);
    EXPECT_EQ(result1.GetRaw().decimal, 500062); // 5.000123 / 2 = 2.5000615 -> 2.500062

    LongQuantity<6> neg_qty1 = LongQuantity<6>::FromRaw({-5, -123}); // -5.000123
    auto neg_result1 = neg_qty1.DivInt(2, RoundModel::Up());
    EXPECT_EQ(neg_result1.GetRaw().integer, -2);
    EXPECT_EQ(neg_result1.GetRaw().decimal, -500062); // -5.000123 /2 = -2.5000615 -> -2.500062

    LongQuantity<6> exact_qty1 = LongQuantity<6>::FromRaw({10, 500}); // 10.000500
    auto exact_result1 = exact_qty1.DivInt(2, RoundModel::Up());
    EXPECT_EQ(exact_result1.GetRaw().integer, 5);
    EXPECT_EQ(exact_result1.GetRaw().decimal, 250); // 10.000500/2=5.00025


    LongQuantity<6> qty2 = LongQuantity<6>::FromRaw({5, 123}); // 5.000123
    auto result2 = qty2.DivInt(2, RoundModel::Down());
    EXPECT_EQ(result2.GetRaw().integer, 2);
    EXPECT_EQ(result2.GetRaw().decimal, 500061); // 5.000123 / 2 = 2.5000615 -> 2.500061

    LongQuantity<6> neg_qty2 = LongQuantity<6>::FromRaw({-5, -123}); // -5.000123
    auto neg_result2 = neg_qty2.DivInt(2, RoundModel::Down());
    EXPECT_EQ(neg_result2.GetRaw().integer, -2);
    EXPECT_EQ(neg_result2.GetRaw().decimal, -500061); // -5.000123 /2 = -2.5000615 -> -2.500061

    LongQuantity<6> exact_qty2 = LongQuantity<6>::FromRaw({10, 500}); // 10.000500
    auto exact_result2 = exact_qty2.DivInt(2, RoundModel::Down());
    EXPECT_EQ(exact_result2.GetRaw().integer, 5);
    EXPECT_EQ(exact_result2.GetRaw().decimal, 250); // 10.000500/2=5.00025

    
    LongQuantity<6> qty3 = LongQuantity<6>::FromRaw({5, 123}); // 5.000123
    auto result3 = qty3.DivInt(2, RoundModel::Near());
    EXPECT_EQ(result3.GetRaw().integer, 2);
    EXPECT_EQ(result3.GetRaw().decimal, 500062); // 5.000123 / 2 = 2.5000615 -> 2.500062

    LongQuantity<6> neg_qty3 = LongQuantity<6>::FromRaw({-5, -123}); // -5.000123
    auto neg_result3 = neg_qty3.DivInt(2, RoundModel::Near());
    EXPECT_EQ(neg_result3.GetRaw().integer, -2);
    EXPECT_EQ(neg_result3.GetRaw().decimal, -500062); // -5.000123 /2 = -2.5000615 -> -2.500062

    LongQuantity<6> exact_qty3 = LongQuantity<6>::FromRaw({10, 500}); // 10.000500
    auto exact_result3 = exact_qty3.DivInt(2, RoundModel::Near());
    EXPECT_EQ(exact_result3.GetRaw().integer, 5);
    EXPECT_EQ(exact_result3.GetRaw().decimal, 250); // 10.000500/2=5.00025
}

TEST(LongQuantityTest, DivisionEdgeCases) {
    LongQuantity<6> qty = LongQuantity<6>::FromRaw({0, 500}); // 0.000500
    auto result = qty.DivInt(2, RoundModel::Up());
    EXPECT_EQ(result.GetRaw().integer, 0);
    EXPECT_EQ(result.GetRaw().decimal, 250); // 0.000500 / 2 = 0.000250

    auto result_neg = qty.DivInt(-2, RoundModel::Up());
    EXPECT_EQ(result_neg.GetRaw().integer, 0);
    EXPECT_EQ(result_neg.GetRaw().decimal, -250); // 0.000500 / -2= -0.000250

    LongQuantity<6> qty1 = LongQuantity<6>::FromRaw({0, 500}); // 0.000500
    auto result1 = qty1.DivInt(2, RoundModel::Near());
    EXPECT_EQ(result1.GetRaw().integer, 0);
    EXPECT_EQ(result1.GetRaw().decimal, 250); // 0.000500 / 2 = 0.000250

    auto result_neg1 = qty1.DivInt(-2, RoundModel::Near());
    EXPECT_EQ(result_neg1.GetRaw().integer, 0);
    EXPECT_EQ(result_neg1.GetRaw().decimal, -250); // 0.000500 / -2= -0.000250
}

TEST(LongQuantityTest, DivisionOverflow) {
    LongQuantity<3> max_qty = LongQuantity<3>::FromRaw({INT64_MAX, 999});
    EXPECT_THROW(max_qty.DivInt(0, RoundModel::Near()), std::invalid_argument);
}

// ---------------------- Price / Price Division ----------------------
TEST(SQtyPxDivPxTest, RoundPositive) {
    {
        Price<0> px1(300);
        Price<0> px2(34);
        auto qty = px1.DivPxToSQty<6>(px2, RoundModel::Up());
        EXPECT_EQ(qty.GetRaw(), 8823530);  // 300 / 34 = 8.823529 41176470 -> UP -> 8.823530
    }
    {
        Price<0> px1(300);
        Price<0> px2(34);
        auto qty = px1.DivPxToSQty<6>(px2, RoundModel::Down());
        EXPECT_EQ(qty.GetRaw(), 8823529);  // 300 / 34 = 8.823529 41176470 -> DOWN -> 8.823529
    }
    {
        Price<0> px1(300);
        Price<0> px2(34);
        auto qty = px1.DivPxToSQty<6>(px2, RoundModel::Near());
        EXPECT_EQ(qty.GetRaw(), 8823529);  // 300 / 34 = 8.823529 41176470 -> NEAR -> 8.823529
    }
    {
        Price<0> px1(300);
        Price<0> px2(35);
        auto qty = px1.DivPxToSQty<6>(px2, RoundModel::Near());
        EXPECT_EQ(qty.GetRaw(), 8571429);  // 300 / 35 = 8.571428 571428571 -> NEAR -> 8.571429
    }
}

TEST(SQtyPxDivPxTest, RoundNegative) {
    {
        Price<0> px1(300);
        Price<0> px2(-34);
        auto qty = px1.DivPxToSQty<6>(px2, RoundModel::Up());
        EXPECT_EQ(qty.GetRaw(), -8823530);  // 300 / 34 = 8.823529 41176470 -> UP -> 8.823530
    }
    {
        Price<0> px1(300);
        Price<0> px2(-34);
        auto qty = px1.DivPxToSQty<6>(px2, RoundModel::Down());
        EXPECT_EQ(qty.GetRaw(), -8823529);  // 300 / 34 = 8.823529 41176470 -> DOWN -> 8.823529
    }
    {
        Price<0> px1(300);
        Price<0> px2(-34);
        auto qty = px1.DivPxToSQty<6>(px2, RoundModel::Near());
        EXPECT_EQ(qty.GetRaw(), -8823529);  // 300 / 34 = 8.823529 41176470 -> NEAR -> 8.823529
    }
    {
        Price<0> px1(300);
        Price<0> px2(-35);
        auto qty = px1.DivPxToSQty<6>(px2, RoundModel::Near());
        EXPECT_EQ(qty.GetRaw(), -8571429);  // 300 / 35 = 8.571428 571428571 -> NEAR -> 8.571429
    }
}

TEST(LQtyPxDivPxTest, RoundPositive) {
    {
        Price<2> px1(300);
        Price<2> px2(34);
        auto qty = LQtyPxDivPx<2, 10>(px1, px2, RoundModel::Up());
        EXPECT_EQ(qty.GetRaw().integer, 8);  // 300 / 34 = 8.8235294117 6470
        EXPECT_EQ(qty.GetRaw().decimal, 8235294118);
    }
    {
        Price<2> px1(300);
        Price<2> px2(34);
        auto qty = LQtyPxDivPx<2, 10>(px1, px2, RoundModel::Down());
        EXPECT_EQ(qty.GetRaw().integer, 8);  // 300 / 34 = 8.8235294117 6470
        EXPECT_EQ(qty.GetRaw().decimal, 8235294117);
    }
    {
        Price<2> px1(300);
        Price<2> px2(34);
        auto qty = LQtyPxDivPx<2, 10>(px1, px2, RoundModel::Near());
        EXPECT_EQ(qty.GetRaw().integer, 8);  // 300 / 34 = 8.8235294117 6470
        EXPECT_EQ(qty.GetRaw().decimal, 8235294118);
    }
    {
        Price<2> px1(300);
        Price<2> px2(35);
        auto qty = LQtyPxDivPx<2, 10>(px1, px2, RoundModel::Near());
        EXPECT_EQ(qty.GetRaw().integer, 8);  // 300 / 35 = 8.5714285714 28571
        EXPECT_EQ(qty.GetRaw().decimal, 5714285714);
    }
}

TEST(LQtyPxDivPxTest, RoundNegative) {
    {
        Price<2> px1(300);
        Price<2> px2(-34);
        auto qty = LQtyPxDivPx<2, 10>(px1, px2, RoundModel::Up());
        EXPECT_EQ(qty.GetRaw().integer, -8);  // 300 / 34 = 8.8235294117 6470
        EXPECT_EQ(qty.GetRaw().decimal, -8235294118);
    }
    {
        Price<2> px1(300);
        Price<2> px2(-34);
        auto qty = LQtyPxDivPx<2, 10>(px1, px2, RoundModel::Down());
        EXPECT_EQ(qty.GetRaw().integer, -8);  // 300 / 34 = 8.8235294117 6470
        EXPECT_EQ(qty.GetRaw().decimal, -8235294117);
    }
    {
        Price<2> px1(300);
        Price<2> px2(-34);
        auto qty = LQtyPxDivPx<2, 10>(px1, px2, RoundModel::Near());
        EXPECT_EQ(qty.GetRaw().integer, -8);  // 300 / 34 = 8.8235294117 6470
        EXPECT_EQ(qty.GetRaw().decimal, -8235294118);
    }
    {
        Price<2> px1(300);
        Price<2> px2(-35);
        auto qty = LQtyPxDivPx<2, 10>(px1, px2, RoundModel::Near());
        EXPECT_EQ(qty.GetRaw().integer, -8);  // 300 / 35 = 8.5714285714 28571
        EXPECT_EQ(qty.GetRaw().decimal, -5714285714);
    }
}

TEST(IntPxDivPxTest, DivisionExact) {
    Price<0> px1(400);
    Price<0> px2(200);
    auto result = px1.DivToInt(px2, RoundModel::Up());
    EXPECT_EQ(result, 2);
}

TEST(IntPxDivPxTest, RoundUpEdgeCase) {
    Price<0> px1(INT64_MAX);
    Price<0> px2(1);
    auto result = px1.DivToInt(px2, RoundModel::Up());
    EXPECT_EQ(result, INT64_MAX);
}

// Test PxPxDivQty with ShortQuantity
TEST(PxPxDivQtyTest, PxPxDivQtyShortRound) {
    {
        Price<2> px = Price<2>::FromRaw(100); // 1.00
        ShortQuantity<6> qty = ShortQuantity<6>::FromRaw(3000000); // 3.000000
        auto result = px.DivSQtyToPx(qty, RoundModel::Up());
        EXPECT_EQ(result.GetRaw(), 34); // 1.00 / 3.000000 = 0.333... -> Up -> 0.34 (34)
    }
    {
        Price<2> px = Price<2>::FromRaw(100); // 1.00
        ShortQuantity<6> qty = ShortQuantity<6>::FromRaw(-3000000); // 3.000000
        auto result = px.DivSQtyToPx(qty, RoundModel::Up());
        EXPECT_EQ(result.GetRaw(), -34); // 1.00 / 3.000000 = 0.333... -> Up -> 0.34 (34)
    }
    {
        Price<2> px = Price<2>::FromRaw(100); // 1.00
        ShortQuantity<6> qty = ShortQuantity<6>::FromRaw(3000000); // 3.000000
        auto result = px.DivSQtyToPx(qty, RoundModel::Down());
        EXPECT_EQ(result.GetRaw(), 33); // 1.00 / 3.000000 = 0.333... -> Down -> 0.33 (33)
    }
    {
        Price<2> px = Price<2>::FromRaw(100); // 1.00
        ShortQuantity<6> qty = ShortQuantity<6>::FromRaw(-3000000); // 3.000000
        auto result = px.DivSQtyToPx(qty, RoundModel::Down());
        EXPECT_EQ(result.GetRaw(), -33); // 1.00 / 3.000000 = 0.333... -> Down -> 0.33 (33)
    }
    {
        Price<2> px = Price<2>::FromRaw(100); // 1.00
        ShortQuantity<6> qty = ShortQuantity<6>::FromRaw(3000000); // 3.000000
        auto result = px.DivSQtyToPx(qty, RoundModel::Near());
        EXPECT_EQ(result.GetRaw(), 33); // 1.00 / 3.000000 = 0.333... -> Near -> 0.33 (34)
    }
    {
        Price<2> px = Price<2>::FromRaw(100); // 1.00
        ShortQuantity<6> qty = ShortQuantity<6>::FromRaw(-3000000); // 3.000000
        auto result = px.DivSQtyToPx(qty, RoundModel::Near());
        EXPECT_EQ(result.GetRaw(), -33); // 1.00 / 3.000000 = 0.333... -> Near -> 0.33 (34)
    }
    {
        Price<2> px = Price<2>::FromRaw(100); // 1.00
        ShortQuantity<6> qty = ShortQuantity<6>::FromRaw(6000000); // 6.000000
        auto result = px.DivSQtyToPx(qty, RoundModel::Near());
        EXPECT_EQ(result.GetRaw(), 17); // 1.00 / 6.000000 = 0.1666... -> Near -> 0.17 (17)
    }
    {
        Price<2> px = Price<2>::FromRaw(100); // 1.00
        ShortQuantity<6> qty = ShortQuantity<6>::FromRaw(-6000000); // 6.000000
        auto result = px.DivSQtyToPx(qty, RoundModel::Near());
        EXPECT_EQ(result.GetRaw(), -17); // 1.00 / 6.000000 = 0.1666... -> Near -> 0.17 (17)
    }
}

// Test PxPxDivQty with LongQuantity
TEST(PxPxDivQtyTest, PxPxDivQtyLongRound) {
    {
        Price<3> px = Price<3>::FromRaw(1000); // 1.000
        LongQuantity<18> qty = LongQuantity<18>::FromRaw({3, 0}); // 3.000000000000000000
        auto result = PxPxDivQty(px, qty, RoundModel::Up());
        EXPECT_EQ(result.GetRaw(), 334); // 1.00 / 3.000000 = 0.3333... -> Up -> 0.334 (334)
    }
    {
        Price<3> px = Price<3>::FromRaw(1000); // 1.000
        LongQuantity<18> qty = LongQuantity<18>::FromRaw({-3, 0}); // 3.000000000000000000
        auto result = PxPxDivQty(px, qty, RoundModel::Up());
        EXPECT_EQ(result.GetRaw(), -334); // 1.00 / 3.000000 = 0.3333... -> Up -> 0.334 (334)
    }
    {
        Price<3> px = Price<3>::FromRaw(1000); // 1.000
        LongQuantity<18> qty = LongQuantity<18>::FromRaw({3, 0}); // 3.000000000000000000
        auto result = PxPxDivQty(px, qty, RoundModel::Down());
        EXPECT_EQ(result.GetRaw(), 333); // 1.00 / 3.000000 = 0.3333... -> Down -> 0.334 (333)
    }
    {
        Price<3> px = Price<3>::FromRaw(1000); // 1.000
        LongQuantity<18> qty = LongQuantity<18>::FromRaw({-3, 0}); // 3.000000000000000000
        auto result = PxPxDivQty(px, qty, RoundModel::Down());
        EXPECT_EQ(result.GetRaw(), -333); // 1.00 / 3.000000 = 0.3333... -> Down -> 0.333 (333)
    }
    {
        Price<3> px = Price<3>::FromRaw(1000); // 1.000
        LongQuantity<18> qty = LongQuantity<18>::FromRaw({3, 0}); // 3.000000000000000000
        auto result = PxPxDivQty(px, qty, RoundModel::Near());
        EXPECT_EQ(result.GetRaw(), 333); // 1.00 / 3.000000 = 0.3333... -> Near -> 0.334 (333)
    }
    {
        Price<3> px = Price<3>::FromRaw(1000); // 1.000
        LongQuantity<18> qty = LongQuantity<18>::FromRaw({-3, 0}); // 3.000000000000000000
        auto result = PxPxDivQty(px, qty, RoundModel::Near());
        EXPECT_EQ(result.GetRaw(), -333); // 1.00 / 3.000000 = 0.3333... -> Near -> 0.333 (333)
    }
    {
        Price<3> px = Price<3>::FromRaw(1000); // 1.000
        LongQuantity<18> qty = LongQuantity<18>::FromRaw({6, 0}); // 6.000000000000000000
        auto result = PxPxDivQty(px, qty, RoundModel::Near());
        EXPECT_EQ(result.GetRaw(), 167); // 1.00 / 6.000000 = 0.16666... -> Near -> 0.167 (167)
    }
    {
        Price<3> px = Price<3>::FromRaw(1000); // 1.000
        LongQuantity<18> qty = LongQuantity<18>::FromRaw({-6, 0}); // 6.000000000000000000
        auto result = PxPxDivQty(px, qty, RoundModel::Near());
        EXPECT_EQ(result.GetRaw(), -167); // 1.00 / 6.000000 = 0.16666... -> Near -> 0.167 (167)
    }
}

// Test division by zero exception
TEST(PxPxDivQtyTest, PxPxDivQtyZeroDivisor) {
    {
        Price<2> px = Price<2>::FromRaw(100);
        ShortQuantity<9> qty = ShortQuantity<9>::FromRaw(0);
        EXPECT_THROW(px.DivSQtyToPx(qty, RoundModel::Up()), std::invalid_argument);
    }
    {
        Price<2> px = Price<2>::FromRaw(100);
        LongQuantity<18> qty = LongQuantity<18>::FromRaw({0, 0});
        EXPECT_THROW(PxPxDivQty(px, qty, RoundModel::Up()), std::invalid_argument);
    }
}

// Test IntQtyDivQty with ShortQuantity
TEST(IntQtyDivQtyTest, IntQtyDivQtyShortRound) {
    {
        ShortQuantity<9> qty1 = ShortQuantity<9>::FromRaw(15000000000); // 15.000000000
        ShortQuantity<9> qty2 = ShortQuantity<9>::FromRaw(4000000000);  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Up());
        EXPECT_EQ(result, 4); // 15 / 4 = 3.75 -> Up -> 4
    }
    {
        ShortQuantity<9> qty1 = ShortQuantity<9>::FromRaw(15000000000); // 15.000000000
        ShortQuantity<9> qty2 = ShortQuantity<9>::FromRaw(-4000000000);  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Up());
        EXPECT_EQ(result, -4); // 15 / 4 = 3.75 -> Up -> 4
    }
    {
        ShortQuantity<9> qty1 = ShortQuantity<9>::FromRaw(15000000000); // 15.000000000
        ShortQuantity<9> qty2 = ShortQuantity<9>::FromRaw(4000000000);  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Down());
        EXPECT_EQ(result, 3); // 15 / 4 = 3.75 -> Down -> 3
    }
    {
        ShortQuantity<9> qty1 = ShortQuantity<9>::FromRaw(15000000000); // 15.000000000
        ShortQuantity<9> qty2 = ShortQuantity<9>::FromRaw(-4000000000);  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Down());
        EXPECT_EQ(result, -3); // 15 / 4 = 3.75 -> Down -> 3
    }
    {
        ShortQuantity<9> qty1 = ShortQuantity<9>::FromRaw(15000000000); // 15.000000000
        ShortQuantity<9> qty2 = ShortQuantity<9>::FromRaw(4000000000);  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Near());
        EXPECT_EQ(result, 4); // 15 / 4 = 3.75 -> Near -> 4
    }
    {
        ShortQuantity<9> qty1 = ShortQuantity<9>::FromRaw(15000000000); // 15.000000000
        ShortQuantity<9> qty2 = ShortQuantity<9>::FromRaw(-4000000000);  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Near());
        EXPECT_EQ(result, -4); // 15 / 4 = 3.75 -> Near -> 4
    }
    {
        ShortQuantity<9> qty1 = ShortQuantity<9>::FromRaw(15000000000); // 15.000000000
        ShortQuantity<9> qty2 = ShortQuantity<9>::FromRaw(7000000000);  // 7.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Near());
        EXPECT_EQ(result, 2); // 15 / 7 = 2.142857 -> Near -> 2
    }
    {
        ShortQuantity<9> qty1 = ShortQuantity<9>::FromRaw(15000000000); // 15.000000000
        ShortQuantity<9> qty2 = ShortQuantity<9>::FromRaw(-7000000000);  // 7.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Near());
        EXPECT_EQ(result, -2); // 15 / 7 = 2.142857 -> Near -> 2
    }
}

// Test IntQtyDivQty with LongQuantity
TEST(IntQtyDivQtyTest, IntQtyDivQtyLongRound) {
    {
        LongQuantity<18> qty1 = LongQuantity<18>::FromRaw({15, 0}); // 15.000000000
        LongQuantity<18> qty2 = LongQuantity<18>::FromRaw({4,0});  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Up());
        EXPECT_EQ(result, 4); // 15 / 4 = 3.75 -> Up -> 4
    }
    {
        LongQuantity<18> qty1 = LongQuantity<18>::FromRaw({15, 0}); // 15.000000000
        LongQuantity<18> qty2 = LongQuantity<18>::FromRaw({-4,0});  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Up());
        EXPECT_EQ(result, -4); // 15 / 4 = 3.75 -> Up -> 4
    }
    {
        LongQuantity<18> qty1 = LongQuantity<18>::FromRaw({15, 0}); // 15.000000000
        LongQuantity<18> qty2 = LongQuantity<18>::FromRaw({4,0});  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Down());
        EXPECT_EQ(result, 3); // 15 / 4 = 3.75 -> Down -> 3
    }
    {
        LongQuantity<18> qty1 = LongQuantity<18>::FromRaw({15, 0}); // 15.000000000
        LongQuantity<18> qty2 = LongQuantity<18>::FromRaw({-4,0});  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Down());
        EXPECT_EQ(result, -3); // 15 / 4 = 3.75 -> Down -> 3
    }
    {
        LongQuantity<18> qty1 = LongQuantity<18>::FromRaw({15, 0}); // 15.000000000
        LongQuantity<18> qty2 = LongQuantity<18>::FromRaw({4,0});  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Near());
        EXPECT_EQ(result, 4); // 15 / 4 = 3.75 -> Near -> 4
    }
    {
        LongQuantity<18> qty1 = LongQuantity<18>::FromRaw({15, 0}); // 15.000000000
        LongQuantity<18> qty2 = LongQuantity<18>::FromRaw({-4,0});  // 4.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Near());
        EXPECT_EQ(result, -4); // 15 / 4 = 3.75 -> Near -> 4
    }
    {
        LongQuantity<18> qty1 = LongQuantity<18>::FromRaw({15, 0}); // 15.000000000
        LongQuantity<18> qty2 = LongQuantity<18>::FromRaw({7,0});  // 7.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Near());
        EXPECT_EQ(result, 2); // 15 / 7 = 2.142857 -> Near -> 2
    }
    {
        LongQuantity<18> qty1 = LongQuantity<18>::FromRaw({15, 0}); // 15.000000000
        LongQuantity<18> qty2 = LongQuantity<18>::FromRaw({-7,0});  // 7.000000000
        auto result = qty1.DivToInt(qty2, RoundModel::Near());
        EXPECT_EQ(result, -2); // 15 / 7 = 2.142857 -> Near -> 2
    }
}

// Test division by zero exception for IntQtyDivQty
TEST(IntQtyDivQtyTest, IntQtyDivQtyZeroDivisor) {
    ShortQuantity<2> qty1 = ShortQuantity<2>::FromRaw(100);
    ShortQuantity<2> qty2 = ShortQuantity<2>::FromRaw(0);
    EXPECT_THROW(qty1.DivToInt(qty2, RoundModel::Up()), std::invalid_argument);
}
