#include <benchmark/benchmark.h>
#include "decimal_cnl.h"

namespace BM_DecimalCNL {

    DecimalCNL Addition() {
        DecimalCNL a = 2.5;
        DecimalCNL b = 3.7;
        return a + b;
    }

    DecimalCNL Subtraction() {
        DecimalCNL a = 5.8;
        DecimalCNL b = 2.4;
        return a - b;
    }

    DecimalCNL Multiplication() {
        DecimalCNL a = 1.5;
        DecimalCNL b = 2.1;
        return a * b;
    }

    DecimalCNL Division() {
        DecimalCNL a = 6.0;
        DecimalCNL b = 2.0;
        return a / b;
    }

    void CompoundAddition() {
        DecimalCNL a = 2.0;
        DecimalCNL b = 3.0;
        a += b;
    }

    void CompoundSubtraction() {
        DecimalCNL a = 5.0;
        DecimalCNL b = 2.0;
        a -= b;
    }

    void CompoundMultiplication() {
        DecimalCNL a = 2.0;
        DecimalCNL b = 3.0;
        a *= b;
    }

    void CompoundDivision() {
        DecimalCNL a = 6.0;
        DecimalCNL b = 2.0;
        a /= b;
    }

    bool Comparison() {
        DecimalCNL a = 2.0;
        DecimalCNL b = 3.0;
        bool r1 = a == b;
        bool r2 = a != b;
        bool r3 = a < b;
        bool r4 = a <= b;
        bool r5 = a > b;
        bool r6 = a >= b;
        return r1 && r2 && r3 && r4 && r5 && r6;
    }

    static void DecimalCNL_Addition(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Addition());
        }
    }

    static void DecimalCNL_Subtraction(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Subtraction());
        }
    }

    static void DecimalCNL_Multiplication(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Multiplication());
        }
    }

    static void DecimalCNL_Division(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Division());
        }
    }

    static void DecimalCNL_CompoundAddition(benchmark::State& state) {
        for (auto _ : state) {
            CompoundAddition();
        }
    }

    static void DecimalCNL_CompoundSubtraction(benchmark::State& state) {
        for (auto _ : state) {
            CompoundSubtraction();
        }
    }

    static void DecimalCNL_CompoundMultiplication(benchmark::State& state) {
        for (auto _ : state) {
            CompoundMultiplication();
        }
    }

    static void DecimalCNL_CompoundDivision(benchmark::State& state) {
        for (auto _ : state) {
            CompoundDivision();
        }
    }

    static void DecimalCNL_Comparison(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Comparison());
        }
    }

    BENCHMARK(DecimalCNL_Addition);
    BENCHMARK(DecimalCNL_Subtraction);
    BENCHMARK(DecimalCNL_Multiplication);
    BENCHMARK(DecimalCNL_Division);
    BENCHMARK(DecimalCNL_CompoundAddition);
    BENCHMARK(DecimalCNL_CompoundSubtraction);
    BENCHMARK(DecimalCNL_CompoundMultiplication);
    BENCHMARK(DecimalCNL_CompoundDivision);
    BENCHMARK(DecimalCNL_Comparison);
}