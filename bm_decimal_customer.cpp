#include <benchmark/benchmark.h>
#include "decimal_customer.h"

namespace BM_DecimalCustomer {

    DecimalCustomer Addition() {
        DecimalCustomer a = DecimalCustomer::fromDouble(2.5);
        DecimalCustomer b = DecimalCustomer::fromDouble(3.7);
        return a + b;
    }

    DecimalCustomer Subtraction() {
        DecimalCustomer a = DecimalCustomer::fromDouble(5.8);
        DecimalCustomer b = DecimalCustomer::fromDouble(2.4);
        return a - b;
    }

    DecimalCustomer Multiplication() {
        DecimalCustomer a = DecimalCustomer::fromDouble(1.5);
        DecimalCustomer b = DecimalCustomer::fromDouble(2.1);
        return a * b;
    }

    DecimalCustomer Division() {
        DecimalCustomer a = DecimalCustomer::fromDouble(6.0);
        DecimalCustomer b = DecimalCustomer::fromDouble(2.0);
        return a / b;
    }

    void CompoundAddition() {
        DecimalCustomer a = DecimalCustomer::fromDouble(2.0);
        DecimalCustomer b = DecimalCustomer::fromDouble(3.0);
        a += b;
    }

    void CompoundSubtraction() {
        DecimalCustomer a = DecimalCustomer::fromDouble(5.0);
        DecimalCustomer b = DecimalCustomer::fromDouble(2.0);
        a -= b;
    }

    void CompoundMultiplication() {
        DecimalCustomer a = DecimalCustomer::fromDouble(2.0);
        DecimalCustomer b = DecimalCustomer::fromDouble(3.0);
        a *= b;
    }

    void CompoundDivision() {
        DecimalCustomer a = DecimalCustomer::fromDouble(6.0);
        DecimalCustomer b = DecimalCustomer::fromDouble(2.0);
        a /= b;
    }

    bool Comparison() {
        DecimalCustomer a = DecimalCustomer::fromDouble(2.0);
        DecimalCustomer b = DecimalCustomer::fromDouble(3.0);
        bool r1 = a == b;
        bool r2 = a != b;
        bool r3 = a < b;
        bool r4 = a <= b;
        bool r5 = a > b;
        bool r6 = a >= b;
        return r1 && r2 && r3 && r4 && r5 && r6;
    }

    static void DecimalCustomer_Addition(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Addition());
        }
    }

    static void DecimalCustomer_Subtraction(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Subtraction());
        }
    }

    static void DecimalCustomer_Multiplication(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Multiplication());
        }
    }

    static void DecimalCustomer_Division(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Division());
        }
    }

    static void DecimalCustomer_CompoundAddition(benchmark::State& state) {
        for (auto _ : state) {
            CompoundAddition();
        }
    }

    static void DecimalCustomer_CompoundSubtraction(benchmark::State& state) {
        for (auto _ : state) {
            CompoundSubtraction();
        }
    }

    static void DecimalCustomer_CompoundMultiplication(benchmark::State& state) {
        for (auto _ : state) {
            CompoundMultiplication();
        }
    }

    static void DecimalCustomer_CompoundDivision(benchmark::State& state) {
        for (auto _ : state) {
            CompoundDivision();
        }
    }

    static void DecimalCustomer_Comparison(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Comparison());
        }
    }

    BENCHMARK(DecimalCustomer_Addition);
    BENCHMARK(DecimalCustomer_Subtraction);
    BENCHMARK(DecimalCustomer_Multiplication);
    BENCHMARK(DecimalCustomer_Division);
    BENCHMARK(DecimalCustomer_CompoundAddition);
    BENCHMARK(DecimalCustomer_CompoundSubtraction);
    BENCHMARK(DecimalCustomer_CompoundMultiplication);
    BENCHMARK(DecimalCustomer_CompoundDivision);
    BENCHMARK(DecimalCustomer_Comparison);
}