#include <gtest/gtest.h>
#include "decimal_boost.h"

TEST(DecimalBoost, Addition) {
    DecimalBoost a = "2.5";
    DecimalBoost b = "3.7";
    DecimalBoost result = a + b;
    DecimalBoost expect = "6.2";
    EXPECT_EQ(result, expect); 
}

TEST(DecimalBoost, Subtraction) {
    DecimalBoost a = "5.8";
    DecimalBoost b = "2.4";
    DecimalBoost result = a - b;
    DecimalBoost expect = "3.4";
    EXPECT_EQ(result, expect); 
}

TEST(DecimalBoost, Multiplication) {
    DecimalBoost a = "1.5";
    DecimalBoost b = "2.1";
    DecimalBoost result = a * b;
    DecimalBoost expect = "3.15";
    EXPECT_EQ(result, expect); 
}

TEST(DecimalBoost, Division) {
    DecimalBoost a = "6.0";
    DecimalBoost b = "2.0";
    DecimalBoost result = a / b;
    DecimalBoost expect = "3";
    EXPECT_EQ(result, expect); 
}

TEST(DecimalBoost, CompoundAddition) {
    DecimalBoost a = "2.0";
    DecimalBoost b = "3.0";
    a += b;
    DecimalBoost expect = "5";
    EXPECT_EQ(a, expect); 
}

TEST(DecimalBoost, CompoundSubtraction) {
    DecimalBoost a = "5.0";
    DecimalBoost b = "2.0";
    a -= b;
    DecimalBoost expect = "3";
    EXPECT_EQ(a, expect); 
}

TEST(DecimalBoost, CompoundMultiplication) {
    DecimalBoost a = "2.0";
    DecimalBoost b = "3.0";
    a *= b;
    DecimalBoost expect = "6";
    EXPECT_EQ(a, expect); 
}

TEST(DecimalBoost, CompoundDivision) {
    DecimalBoost a = "6.0";
    DecimalBoost b = "2.0";
    a /= b;
    DecimalBoost expect = "3";
    EXPECT_EQ(a, expect); 
}

TEST(DecimalBoost, Comparison) {
    DecimalBoost a = "2.0";
    DecimalBoost b = "3.0";
    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
    EXPECT_LE(a, b);
    EXPECT_GE(b, a);
    EXPECT_NE(a, b);
}
