#include <benchmark/benchmark.h>
#include "decimal_fix16.h"

namespace BM_DecimalFix16 {

    DecimalFix16 Addition() {
        DecimalFix16 a = 2.5;
        DecimalFix16 b = 3.7;
        return a + b;
    }

    DecimalFix16 Subtraction() {
        DecimalFix16 a = 5.8;
        DecimalFix16 b = 2.4;
        return a - b;
    }

    DecimalFix16 Multiplication() {
        DecimalFix16 a = 1.5;
        DecimalFix16 b = 2.1;
        return a * b;
    }

    DecimalFix16 Division() {
        DecimalFix16 a = 6.0;
        DecimalFix16 b = 2.0;
        return a / b;
    }

    void CompoundAddition() {
        DecimalFix16 a = 2.0;
        DecimalFix16 b = 3.0;
        a += b;
    }

    void CompoundSubtraction() {
        DecimalFix16 a = 5.0;
        DecimalFix16 b = 2.0;
        a -= b;
    }

    void CompoundMultiplication() {
        DecimalFix16 a = 2.0;
        DecimalFix16 b = 3.0;
        a *= b;
    }

    void CompoundDivision() {
        DecimalFix16 a = 6.0;
        DecimalFix16 b = 2.0;
        a /= b;
    }

    bool Comparison() {
        DecimalFix16 a = 2.0;
        DecimalFix16 b = 3.0;
        bool r1 = a == b;
        bool r2 = a != b;
        bool r3 = a < b;
        bool r4 = a <= b;
        bool r5 = a > b;
        bool r6 = a >= b;
        return r1 && r2 && r3 && r4 && r5 && r6;
    }

    static void DecimalFix16_Addition(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Addition());
        }
    }

    static void DecimalFix16_Subtraction(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Subtraction());
        }
    }

    static void DecimalFix16_Multiplication(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Multiplication());
        }
    }

    static void DecimalFix16_Division(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Division());
        }
    }

    static void DecimalFix16_CompoundAddition(benchmark::State& state) {
        for (auto _ : state) {
            CompoundAddition();
        }
    }

    static void DecimalFix16_CompoundSubtraction(benchmark::State& state) {
        for (auto _ : state) {
            CompoundSubtraction();
        }
    }

    static void DecimalFix16_CompoundMultiplication(benchmark::State& state) {
        for (auto _ : state) {
            CompoundMultiplication();
        }
    }

    static void DecimalFix16_CompoundDivision(benchmark::State& state) {
        for (auto _ : state) {
            CompoundDivision();
        }
    }

    static void DecimalFix16_Comparison(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Comparison());
        }
    }

    BENCHMARK(DecimalFix16_Addition);
    BENCHMARK(DecimalFix16_Subtraction);
    BENCHMARK(DecimalFix16_Multiplication);
    BENCHMARK(DecimalFix16_Division);
    BENCHMARK(DecimalFix16_CompoundAddition);
    BENCHMARK(DecimalFix16_CompoundSubtraction);
    BENCHMARK(DecimalFix16_CompoundMultiplication);
    BENCHMARK(DecimalFix16_CompoundDivision);
    BENCHMARK(DecimalFix16_Comparison);
}