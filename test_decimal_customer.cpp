#include <gtest/gtest.h>
#include "decimal_customer.h"

TEST(DecimalCustomer, Addition) {
    DecimalCustomer a = DecimalCustomer::fromDouble(2.5);
    DecimalCustomer b = DecimalCustomer::fromDouble(3.7);
    DecimalCustomer result = a + b;
    DecimalCustomer expect = DecimalCustomer::fromDouble(6.2);
    EXPECT_EQ(result, expect); 
}

TEST(DecimalCustomer, Subtraction) {
    DecimalCustomer a = DecimalCustomer::fromDouble(5.8);
    DecimalCustomer b = DecimalCustomer::fromDouble(2.4);
    DecimalCustomer result = a - b;
    DecimalCustomer expect = DecimalCustomer::fromDouble(3.4);
    EXPECT_EQ(result, expect);
}

TEST(DecimalCustomer, Multiplication) {
    DecimalCustomer a = DecimalCustomer::fromDouble(1.5);
    DecimalCustomer b = DecimalCustomer::fromDouble(2.1);
    DecimalCustomer result = a * b;
    DecimalCustomer expect = DecimalCustomer::fromDouble(3.15);
    EXPECT_EQ(result, expect); 
}

TEST(DecimalCustomer, Division) {
    DecimalCustomer a = DecimalCustomer::fromDouble(6.0);
    DecimalCustomer b = DecimalCustomer::fromDouble(2.0);
    DecimalCustomer result = a / b;
    DecimalCustomer expect = DecimalCustomer::fromDouble(3);
    EXPECT_EQ(result, expect); 
}

TEST(DecimalCustomer, CompoundAddition) {
    DecimalCustomer a = DecimalCustomer::fromDouble(2.0);
    DecimalCustomer b = DecimalCustomer::fromDouble(3.0);
    a += b;
    DecimalCustomer expect = DecimalCustomer::fromDouble(5);
    EXPECT_EQ(a, expect); 
}

TEST(DecimalCustomer, CompoundSubtraction) {
    DecimalCustomer a = DecimalCustomer::fromDouble(5.0);
    DecimalCustomer b = DecimalCustomer::fromDouble(2.0);
    a -= b;
    DecimalCustomer expect = DecimalCustomer::fromDouble(3);
    EXPECT_EQ(a, expect); 
}

TEST(DecimalCustomer, CompoundMultiplication) {
    DecimalCustomer a = DecimalCustomer::fromDouble(2.0);
    DecimalCustomer b = DecimalCustomer::fromDouble(3.0);
    a *= b;
    DecimalCustomer expect = DecimalCustomer::fromDouble(6);
    EXPECT_EQ(a, expect); 
}

TEST(DecimalCustomer, CompoundDivision) {
    DecimalCustomer a = DecimalCustomer::fromDouble(6.0);
    DecimalCustomer b = DecimalCustomer::fromDouble(2.0);
    a /= b;
    DecimalCustomer expect = DecimalCustomer::fromDouble(3);
    EXPECT_EQ(a, expect); 
}

TEST(DecimalCustomer, Comparison) {
    DecimalCustomer a = DecimalCustomer::fromDouble(2.0);
    DecimalCustomer b = DecimalCustomer::fromDouble(3.0);
    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
    EXPECT_LE(a, b);
    EXPECT_GE(b, a);
    EXPECT_NE(a, b);
}
