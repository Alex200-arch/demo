#include <gtest/gtest.h>
#include "../decimal64.hpp"

using Decimal_t = Decimal<0>;

TEST(DecimalTest, DefaultConstructor) {
    Decimal_t::NEAR d;
    EXPECT_EQ(d.GetRawValue(), 0);
}

TEST(DecimalTest, FromInteger) {
    using Decimal_t = Decimal<1>;
    Decimal_t::NEAR d1 = Decimal_t::NEAR::FromInteger(123);
    EXPECT_EQ(d1.GetRawValue(), 123 * Decimal_t::SCALING_FACTOR);

    Decimal_t::NEAR d2 = Decimal_t::NEAR::FromInteger(-456);
    EXPECT_EQ(d2.GetRawValue(), -456 * Decimal_t::SCALING_FACTOR);

    // Test integer overflow
    EXPECT_THROW(Decimal_t::NEAR::FromInteger(INT64_MAX / Decimal_t::SCALING_FACTOR + 1), std::overflow_error);
    EXPECT_THROW(Decimal_t::NEAR::FromInteger(INT64_MIN / Decimal_t::SCALING_FACTOR - 1), std::overflow_error);
}

TEST(DecimalTest, FromDouble) {
    Decimal_t::NEAR d1 = Decimal_t::NEAR::FromDouble(123.45678901);
    EXPECT_NEAR(d1.ToDouble(), 123.45678901, Decimal_t::ABS_ERROR);

    Decimal_t::NEAR d2 = Decimal_t::NEAR::FromDouble(-987.65432109);
    EXPECT_NEAR(d2.ToDouble(), -987.65432109, Decimal_t::ABS_ERROR);

    // Test double overflow
    EXPECT_THROW(Decimal_t::NEAR::FromDouble(1e308), std::overflow_error);
    EXPECT_THROW(Decimal_t::NEAR::FromDouble(-1e308), std::overflow_error);
}

TEST(DecimalTest, ArithmeticOperations) {
    Decimal_t::NEAR a = Decimal_t::NEAR::FromInteger(100);
    Decimal_t::NEAR b = Decimal_t::NEAR::FromInteger(50);
    
    EXPECT_EQ((a + b).GetRawValue(), 150 * Decimal_t::SCALING_FACTOR);
    EXPECT_EQ((a - b).GetRawValue(), 50 * Decimal_t::SCALING_FACTOR);
    EXPECT_EQ((a * b).GetRawValue(), 5000 * Decimal_t::SCALING_FACTOR);
    EXPECT_EQ((a / b).GetRawValue(), 2 * Decimal_t::SCALING_FACTOR);
}

TEST(DecimalTest, AdditionOverflow) {
    Decimal_t::NEAR maxVal = Decimal_t::NEAR::FromRaw(INT64_MAX);
    Decimal_t::NEAR minVal = Decimal_t::NEAR::FromRaw(INT64_MIN);
    Decimal_t::NEAR one = Decimal_t::NEAR::FromInteger(1);
    Decimal_t::NEAR minus_one = Decimal_t::NEAR::FromInteger(-1);
    
    EXPECT_THROW(maxVal + one, std::overflow_error);
    EXPECT_THROW(minVal + minus_one, std::overflow_error);
}

TEST(DecimalTest, SubtractionOverflow) {
    Decimal_t::NEAR maxVal = Decimal_t::NEAR::FromRaw(INT64_MAX);
    Decimal_t::NEAR minVal = Decimal_t::NEAR::FromRaw(INT64_MIN);
    Decimal_t::NEAR one = Decimal_t::NEAR::FromInteger(1);
    Decimal_t::NEAR minus_one = Decimal_t::NEAR::FromInteger(-1);
    
    EXPECT_THROW(minVal - one, std::overflow_error);
    EXPECT_THROW(maxVal - minus_one, std::overflow_error);
}

TEST(DecimalTest, MultiplicationPositiveRoundUp) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::UP a = Decimal_t::UP::FromDouble(1.23);
    // Decimal_t::UP b = Decimal_t::UP::FromDouble(4.56);
    Decimal_t::UP a = Decimal_t::UP::FromRaw(123);
    Decimal_t::UP b = Decimal_t::UP::FromRaw(456);
    auto c = a * b;
    EXPECT_EQ(c.GetRawValue(), 561); // 1.23*4.56=5.6088 → 5.61
}

TEST(DecimalTest, MultiplicationEdgeNoRound) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::UP a = Decimal_t::UP::FromDouble(1.01);
    // Decimal_t::UP b = Decimal_t::UP::FromDouble(1.01);
    Decimal_t::UP a = Decimal_t::UP::FromRaw(101);
    Decimal_t::UP b = Decimal_t::UP::FromRaw(101);
    auto c = a * b;
    EXPECT_EQ(c.GetRawValue(), 102); // 1.01*1.01=1.0201 → 1.02
}

TEST(DecimalTest, MultiplicationNegativeRoundUp) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::UP a = Decimal_t::UP::FromDouble(-1.23);
    // Decimal_t::UP b = Decimal_t::UP::FromDouble(4.56);
    Decimal_t::UP a = Decimal_t::UP::FromRaw(-123);
    Decimal_t::UP b = Decimal_t::UP::FromRaw(456);
    auto c = a * b;
    EXPECT_EQ(c.GetRawValue(), -561); // -1.23*4.56=-5.6088 → -5.61 (more negative)
}

TEST(DecimalTest, MultiplicationTruncate) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::DOWN a = Decimal_t::DOWN::FromDouble(1.23);
    // Decimal_t::DOWN b = Decimal_t::DOWN::FromDouble(4.56);
    Decimal_t::DOWN a = Decimal_t::DOWN::FromRaw(123);
    Decimal_t::DOWN b = Decimal_t::DOWN::FromRaw(456);
    auto c = a * b;
    EXPECT_EQ(c.GetRawValue(), 560); // 1.23*4.56=5.6088 → 5.60
}

TEST(DecimalTest, MultiplicationNegativeTruncate) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::DOWN a = Decimal_t::DOWN::FromDouble(-1.23);
    // Decimal_t::DOWN b = Decimal_t::DOWN::FromDouble(4.56);
    Decimal_t::DOWN a = Decimal_t::DOWN::FromRaw(-123);
    Decimal_t::DOWN b = Decimal_t::DOWN::FromRaw(456);
    auto c = a * b;
    EXPECT_EQ(c.GetRawValue(), -560); // -1.23*4.56-5.6088 → -5.60
}

TEST(DecimalTest, MultiplicationRoundHalfUp) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::NEAR a = Decimal_t::NEAR::FromDouble(1.25);
    // Decimal_t::NEAR b = Decimal_t::NEAR::FromDouble(1.25);
    Decimal_t::NEAR a = Decimal_t::NEAR::FromRaw(125);
    Decimal_t::NEAR b = Decimal_t::NEAR::FromRaw(125);
    auto c = a * b;
    EXPECT_EQ(c.GetRawValue(), 156); // 1.25*1.25=1.5625 → 1.56 (nearest)
}

TEST(DecimalTest, MultiplicationRoundHalfDown) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::NEAR a =  Decimal_t::NEAR::FromDouble(1.24);
    // Decimal_t::NEAR b =  Decimal_t::NEAR::FromDouble(1.24);
    Decimal_t::NEAR a =  Decimal_t::NEAR::FromRaw(124);
    Decimal_t::NEAR b =  Decimal_t::NEAR::FromRaw(124);
    auto c = a * b; // 1.24*1.24=1.5376 → 1.54 (nearest)
    EXPECT_EQ(c.GetRawValue(), 154);
}

TEST(DecimalTest, MultiplicationOverflow) {
    using Decimal_t = Decimal<0>;
    Decimal_t::UP a = Decimal_t::UP::FromInteger(INT64_MAX);
    Decimal_t::UP b = Decimal_t::UP::FromInteger(2);
    EXPECT_THROW(a * b, std::overflow_error);
}

TEST(DecimalTest, IntegerMultiplication) {
    using Decimal_t = Decimal<0>;
    Decimal_t::UP a = Decimal_t::UP::FromInteger(25646);
    Decimal_t::UP b = Decimal_t::UP::FromInteger(46677934);
    auto c = a * b;
    EXPECT_EQ(c.GetRawValue(), 1197102295364);
}

TEST(DecimalTest, IntegerDivision_1) {
    using Decimal_t = Decimal<0>;
    Decimal_t::UP a = Decimal_t::UP::FromInteger(10);
    Decimal_t::UP b = Decimal_t::UP::FromInteger(3);
    auto c = a / b;
    EXPECT_EQ(c.GetRawValue(), 4);
}

TEST(DecimalTest, IntegerDivision_2) {
    using Decimal_t = Decimal<0>;
    Decimal_t::DOWN a = Decimal_t::DOWN::FromInteger(10);
    Decimal_t::DOWN b = Decimal_t::DOWN::FromInteger(3);
    auto c = a / b;
    EXPECT_EQ(c.GetRawValue(), 3);
}

TEST(DecimalTest, IntegerDivision_3) {
    using Decimal_t = Decimal<0>;
    Decimal_t::NEAR a = Decimal_t::NEAR::FromInteger(10);
    Decimal_t::NEAR b = Decimal_t::NEAR::FromInteger(3);
    auto c = a / b;
    EXPECT_EQ(c.GetRawValue(), 3);
}

TEST(DecimalTest, DivisionRoundUp) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::UP a = Decimal_t::UP::FromDouble(1.23);
    // Decimal_t::UP b = Decimal_t::UP::FromDouble(0.45);
    Decimal_t::UP a = Decimal_t::UP::FromRaw(123);
    Decimal_t::UP b = Decimal_t::UP::FromRaw(45);
    auto c = a / b;
    EXPECT_EQ(c.GetRawValue(), 274); // 1.23/0.45=2.733... → 2.74
}

TEST(DecimalTest, DivisionNegativeRoundUp) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::UP a = Decimal_t::UP::FromDouble(-1.23);
    // Decimal_t::UP b = Decimal_t::UP::FromDouble(0.45);
    Decimal_t::UP a = Decimal_t::UP::FromRaw(-123);
    Decimal_t::UP b = Decimal_t::UP::FromRaw(45);
    auto c = a / b;
    EXPECT_EQ(c.GetRawValue(), -274); // -1.23/0.45=-2.733... → -2.74 (more negative)
}

TEST(DecimalTest, DivisionTruncate) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::DOWN a = Decimal_t::DOWN::FromDouble(1.23);
    // Decimal_t::DOWN b = Decimal_t::DOWN::FromDouble(0.45);
    Decimal_t::DOWN a = Decimal_t::DOWN::FromRaw(123);
    Decimal_t::DOWN b = Decimal_t::DOWN::FromRaw(45);
    auto c = a / b;
    EXPECT_EQ(c.GetRawValue(), 273); // 1.23/0.45=2.733... → 2.73
}

TEST(DecimalTest, DivisionExactNoTruncate) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::DOWN a = Decimal_t::DOWN::FromDouble(2.00);
    // Decimal_t::DOWN b = Decimal_t::DOWN::FromDouble(0.50);
    Decimal_t::DOWN a = Decimal_t::DOWN::FromRaw(200);
    Decimal_t::DOWN b = Decimal_t::DOWN::FromRaw(50);
    auto c = a / b;
    EXPECT_EQ(c.GetRawValue(), 400); // 2/0.5=4.00
}

TEST(DecimalTest, DivisionRoundHalfEven) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::NEAR a = Decimal_t::NEAR::FromDouble(2.00);
    // Decimal_t::NEAR b =Decimal_t::NEAR::FromDouble(3.00);
    Decimal_t::NEAR a = Decimal_t::NEAR::FromRaw(200);
    Decimal_t::NEAR b = Decimal_t::NEAR::FromRaw(300);
    auto c = a / b; // 2/3=0.666... → 0.67 (rounded up)
    EXPECT_EQ(c.GetRawValue(), 67);
}

TEST(DecimalTest, DivisionRoundDown) {
    using Decimal_t = Decimal<2>;
    // Decimal_t::NEAR a = Decimal_t::NEAR::FromDouble(1.99);
    // Decimal_t::NEAR b = Decimal_t::NEAR::FromDouble(2.00);
    Decimal_t::NEAR a = Decimal_t::NEAR::FromRaw(199);
    Decimal_t::NEAR b = Decimal_t::NEAR::FromRaw(200);
    auto c = a / b; // 1.99/2=0.995 → 1.00 (rounded up)
    EXPECT_EQ(c.GetRawValue(), 100);
}

TEST(DecimalTest, DivisionOverflow) {
    using Decimal_t = Decimal<0>;
    Decimal_t::UP a = Decimal_t::UP::FromInteger(INT64_MIN);
    Decimal_t::UP b = Decimal_t::UP::FromInteger(-1);
    EXPECT_THROW(a / b, std::overflow_error); // INT64_MIN / -1 overflows
}

TEST(DecimalTest, DivisionRounding_1) {
    using Decimal_t = Decimal<8>;
    Decimal_t::NEAR a = Decimal_t::NEAR::FromInteger(10); // 10.0
    Decimal_t::NEAR b = Decimal_t::NEAR::FromInteger(3);  // 3.0
    // 10 / 3 = 3.33333333333...... with different rounding

    Decimal_t::UP res1 = Decimal_t::UP(a.GetRawValue()) / Decimal_t::UP(b.GetRawValue());
    EXPECT_EQ(res1.GetRawValue(), 333333334); // 3.33333334

    Decimal_t::DOWN res2 = Decimal_t::DOWN(a.GetRawValue()) / Decimal_t::DOWN(b.GetRawValue());
    EXPECT_EQ(res2.GetRawValue(), 333333333); // 3.33333333

    Decimal_t::NEAR res3 = a / b;
    EXPECT_EQ(res3.GetRawValue(), 333333333); // 3.33333333
}

TEST(DecimalTest, DivisionRounding_2) {
    using Decimal_t = Decimal<8>;
    Decimal_t::NEAR a = Decimal_t::NEAR::FromInteger(10); // 10.0
    Decimal_t::NEAR b = Decimal_t::NEAR::FromInteger(6);  // 6.0
    // 10 / 6 = 1.6666666666...... with different rounding

    Decimal_t::UP res1 = Decimal_t::UP(a.GetRawValue()) / Decimal_t::UP(b.GetRawValue());
    EXPECT_EQ(res1.GetRawValue(), 166666667); // 1.66666667

    Decimal_t::DOWN res2 = Decimal_t::DOWN(a.GetRawValue()) / Decimal_t::DOWN(b.GetRawValue());
    EXPECT_EQ(res2.GetRawValue(), 166666666); // 1.66666666

    Decimal_t::NEAR res3 = a / b;
    EXPECT_EQ(res3.GetRawValue(), 166666667); // 1.66666667
}

TEST(DecimalTest, DivisionRounding_3) {
    using Decimal_t = Decimal<8>;
    Decimal_t::NEAR a = Decimal_t::NEAR::FromRaw(266666666020);
    Decimal_t::NEAR b = Decimal_t::NEAR::FromRaw(200000000000);
    // 2666.6666602 / 2000 = 1.3333333301...... with different rounding

    Decimal_t::UP res1 = Decimal_t::UP(a.GetRawValue()) / Decimal_t::UP(b.GetRawValue());
    EXPECT_EQ(res1.GetRawValue(), 133333333); // 1.33333333

    Decimal_t::DOWN res2 = Decimal_t::DOWN(a.GetRawValue()) / Decimal_t::DOWN(b.GetRawValue());
    EXPECT_EQ(res2.GetRawValue(), 133333333); // 1.33333333

    Decimal_t::NEAR res3 = a / b;
    EXPECT_EQ(res3.GetRawValue(), 133333333); // 1.33333333
}

TEST(DecimalTest, DivisionByZero) {
    Decimal_t::NEAR a = Decimal_t::NEAR::FromInteger(1);
    Decimal_t::NEAR zero = Decimal_t::NEAR::FromInteger(0);
    EXPECT_THROW(a / zero, std::invalid_argument);
}

TEST(DecimalTest, ComparisonOperators) {
    Decimal_t::NEAR a = Decimal_t::NEAR::FromInteger(100);
    Decimal_t::NEAR b = Decimal_t::NEAR::FromInteger(200);
    Decimal_t::NEAR c = Decimal_t::NEAR::FromInteger(100);

    EXPECT_TRUE(a == c);
    EXPECT_TRUE(a != b);
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= c);
    EXPECT_TRUE(b >= a);
}

TEST(DecimalTest, CompoundAssignment) {
    Decimal_t::NEAR a = Decimal_t::NEAR::FromInteger(100);
    a += Decimal_t::NEAR::FromInteger(50);
    EXPECT_EQ(a.GetRawValue(), 150 * Decimal_t::SCALING_FACTOR);

    a -= Decimal_t::NEAR::FromInteger(30);
    EXPECT_EQ(a.GetRawValue(), 120 * Decimal_t::SCALING_FACTOR);

    a *= Decimal_t::NEAR::FromInteger(2);
    EXPECT_EQ(a.GetRawValue(), 240 * Decimal_t::SCALING_FACTOR);

    a /= Decimal_t::NEAR::FromInteger(3);
    EXPECT_EQ(a.GetRawValue(), 80 * Decimal_t::SCALING_FACTOR);
}

TEST(DecimalTest, EdgeCases) {
    // Max value test
    Decimal_t::NEAR maxVal = Decimal_t::NEAR::FromRaw(INT64_MAX);
    EXPECT_THROW(maxVal * maxVal, std::overflow_error);

    // Min value test
    Decimal_t::NEAR minVal = Decimal_t::NEAR::FromRaw(INT64_MIN);
    EXPECT_THROW(minVal * Decimal_t::NEAR::FromInteger(-1), std::overflow_error);

    // Zero behavior
    Decimal_t::NEAR zero = Decimal_t::NEAR::FromInteger(0);
    EXPECT_EQ((zero * maxVal).GetRawValue(), 0);
    EXPECT_EQ((zero / maxVal).GetRawValue(), 0);
}

TEST(DecimalTest, ToDoublePrecision) {
    Decimal_t::NEAR d = Decimal_t::NEAR::FromRaw(123456789);
    EXPECT_NEAR(d.ToDouble(), (double)123456789 / Decimal_t::SCALING_FACTOR, Decimal_t::ABS_ERROR);
    
    Decimal_t::NEAR d2 = Decimal_t::NEAR::FromRaw(-9876543210987);
    EXPECT_NEAR(d2.ToDouble(), (double)-9876543210987 / Decimal_t::SCALING_FACTOR, Decimal_t::ABS_ERROR);
}

TEST(DecimalTest, NegativeNumberOperations_1) {
    Decimal_t::UP a = Decimal_t::UP::FromInteger(-100);
    Decimal_t::UP b = Decimal_t::UP::FromInteger(50);
    
    EXPECT_EQ((a + b).GetRawValue(), (-50 * Decimal_t::SCALING_FACTOR));
    EXPECT_EQ((a - b).GetRawValue(), (-150 * Decimal_t::SCALING_FACTOR));
    EXPECT_EQ((a * b).GetRawValue(), (-5000 * Decimal_t::SCALING_FACTOR));
    EXPECT_EQ((a / b).GetRawValue(), (-2 * Decimal_t::SCALING_FACTOR));
}

TEST(DecimalTest, NegativeNumberOperations_2) {
    Decimal_t::DOWN a = Decimal_t::DOWN::FromInteger(-100);
    Decimal_t::DOWN b = Decimal_t::DOWN::FromInteger(50);
    
    EXPECT_EQ((a + b).GetRawValue(), (-50 * Decimal_t::SCALING_FACTOR));
    EXPECT_EQ((a - b).GetRawValue(), (-150 * Decimal_t::SCALING_FACTOR));
    EXPECT_EQ((a * b).GetRawValue(), (-5000 * Decimal_t::SCALING_FACTOR));
    EXPECT_EQ((a / b).GetRawValue(), (-2 * Decimal_t::SCALING_FACTOR));
}

TEST(DecimalTest, NegativeNumberOperations_3) {
    Decimal_t::NEAR a = Decimal_t::NEAR::FromInteger(-100);
    Decimal_t::NEAR b = Decimal_t::NEAR::FromInteger(50);
    
    EXPECT_EQ((a + b).GetRawValue(), (-50 * Decimal_t::SCALING_FACTOR));
    EXPECT_EQ((a - b).GetRawValue(), (-150 * Decimal_t::SCALING_FACTOR));
    EXPECT_EQ((a * b).GetRawValue(), (-5000 * Decimal_t::SCALING_FACTOR));
    EXPECT_EQ((a / b).GetRawValue(), (-2 * Decimal_t::SCALING_FACTOR));
}