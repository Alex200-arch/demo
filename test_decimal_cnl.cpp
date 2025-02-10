#include <gtest/gtest.h>
#include "decimal_cnl.h"

TEST(DecimalCNL, Addition) {
    DecimalCNL a = 2.5;
    DecimalCNL b = 3.7;
    DecimalCNL result = a + b;
    DecimalCNL expect = 6.2;
    EXPECT_EQ(result, expect); 
}

TEST(DecimalCNL, Subtraction) {
    DecimalCNL a = 5.8;
    DecimalCNL b = 2.4;
    DecimalCNL result = a - b;
    DecimalCNL expect = 3.4;
    EXPECT_EQ(result, expect);
}

TEST(DecimalCNL, Multiplication) {
    DecimalCNL a = 1.5;
    DecimalCNL b = 2.1;
    DecimalCNL result = a * b;
    DecimalCNL expect = 3.15;
    EXPECT_EQ(result, expect); 
}

TEST(DecimalCNL, Division) {
    DecimalCNL a = 6.0;
    DecimalCNL b = 2.0;
    DecimalCNL result = a / b;
    DecimalCNL expect = 3;
    EXPECT_EQ(result, expect); 
}

TEST(DecimalCNL, CompoundAddition) {
    DecimalCNL a = 2.0;
    DecimalCNL b = 3.0;
    a += b;
    DecimalCNL expect = 5;
    EXPECT_EQ(a, expect); 
}

TEST(DecimalCNL, CompoundSubtraction) {
    DecimalCNL a = 5.0;
    DecimalCNL b = 2.0;
    a -= b;
    DecimalCNL expect = 3;
    EXPECT_EQ(a, expect); 
}

TEST(DecimalCNL, CompoundMultiplication) {
    DecimalCNL a = 2.0;
    DecimalCNL b = 3.0;
    a *= b;
    DecimalCNL expect = 6;
    EXPECT_EQ(a, expect); 
}

TEST(DecimalCNL, CompoundDivision) {
    DecimalCNL a = 6.0;
    DecimalCNL b = 2.0;
    a /= b;
    DecimalCNL expect = 3;
    EXPECT_EQ(a, expect); 
}

TEST(DecimalCNL, Comparison) {
    DecimalCNL a = 2.0;
    DecimalCNL b = 3.0;
    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
    EXPECT_LE(a, b);
    EXPECT_GE(b, a);
    EXPECT_NE(a, b);
}
