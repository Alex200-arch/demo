#include <gtest/gtest.h>
#include "decimal_fix16.h"

TEST(DecimalFix16, Addition) {
    DecimalFix16 a = 2.5;
    DecimalFix16 b = 3.7;
    DecimalFix16 result = a + b;
    DecimalFix16 expect = 6.2;
    EXPECT_EQ(result, expect); 
}

TEST(DecimalFix16, Subtraction) {
    DecimalFix16 a = 5.8;
    DecimalFix16 b = 2.0;
    DecimalFix16 result = a - b;
    DecimalFix16 expect = 3.8;
    EXPECT_EQ(result, expect);
}

TEST(DecimalFix16, Multiplication) {
    DecimalFix16 a = 1.5;
    DecimalFix16 b = 2.0;
    DecimalFix16 result = a * b;
    DecimalFix16 expect = 3.0;
    EXPECT_EQ(result, expect); 
}

TEST(DecimalFix16, Division) {
    DecimalFix16 a = 6.0;
    DecimalFix16 b = 2.0;
    DecimalFix16 result = a / b;
    DecimalFix16 expect = 3.0;
    EXPECT_EQ(result, expect); 
}

TEST(DecimalFix16, CompoundAddition) {
    DecimalFix16 a = 2.0;
    DecimalFix16 b = 3.0;
    a += b;
    DecimalFix16 expect = 5.0;
    EXPECT_EQ(a, expect); 
}

TEST(DecimalFix16, CompoundSubtraction) {
    DecimalFix16 a = 5.0;
    DecimalFix16 b = 2.0;
    a -= b;
    DecimalFix16 expect = 3.0;
    EXPECT_EQ(a, expect); 
}

TEST(DecimalFix16, CompoundMultiplication) {
    DecimalFix16 a = 2.0;
    DecimalFix16 b = 3.0;
    a *= b;
    DecimalFix16 expect = 6.0;
    EXPECT_EQ(a, expect); 
}

TEST(DecimalFix16, CompoundDivision) {
    DecimalFix16 a = 6.0;
    DecimalFix16 b = 2.0;
    a /= b;
    DecimalFix16 expect = 3.0;
    EXPECT_EQ(a, expect); 
}

TEST(DecimalFix16, Comparison) {
    DecimalFix16 a = 2.0;
    DecimalFix16 b = 3.0;
    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
    EXPECT_LE(a, b);
    EXPECT_GE(b, a);
    EXPECT_NE(a, b);
}
