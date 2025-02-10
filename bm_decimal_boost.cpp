#include <benchmark/benchmark.h>
#include "decimal_boost.h"

namespace BM_DecimalBoost {

    DecimalBoost Addition() {
        DecimalBoost a = "2.5";
        DecimalBoost b = "3.7";
        return a + b;
    }

    DecimalBoost Subtraction() {
        DecimalBoost a = "5.8";
        DecimalBoost b = "2.4";
        return a - b;
    }

    DecimalBoost Multiplication() {
        DecimalBoost a = "1.5";
        DecimalBoost b = "2.1";
        return a * b;
    }

    DecimalBoost Division() {
        DecimalBoost a = "6.0";
        DecimalBoost b = "2.0";
        return a / b;
    }

    void CompoundAddition() {
        DecimalBoost a = "2.0";
        DecimalBoost b = "3.0";
        a += b;
    }

    void CompoundSubtraction() {
        DecimalBoost a = "5.0";
        DecimalBoost b = "2.0";
        a -= b;
    }

    void CompoundMultiplication() {
        DecimalBoost a = "2.0";
        DecimalBoost b = "3.0";
        a *= b;
    }

    void CompoundDivision() {
        DecimalBoost a = "6.0";
        DecimalBoost b = "2.0";
        a /= b;
    }

    bool Comparison() {
        DecimalBoost a = "2.0";
        DecimalBoost b = "3.0";
        bool r1 = a == b;
        bool r2 = a != b;
        bool r3 = a < b;
        bool r4 = a <= b;
        bool r5 = a > b;
        bool r6 = a >= b;
        return r1 && r2 && r3 && r4 && r5 && r6;
    }

    static void DecimalBoost_Addition(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Addition());
        }
    }

    static void DecimalBoost_Subtraction(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Subtraction());
        }
    }

    static void DecimalBoost_Multiplication(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Multiplication());
        }
    }

    static void DecimalBoost_Division(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Division());
        }
    }

    static void DecimalBoost_CompoundAddition(benchmark::State& state) {
        for (auto _ : state) {
            CompoundAddition();
        }
    }

    static void DecimalBoost_CompoundSubtraction(benchmark::State& state) {
        for (auto _ : state) {
            CompoundSubtraction();
        }
    }

    static void DecimalBoost_CompoundMultiplication(benchmark::State& state) {
        for (auto _ : state) {
            CompoundMultiplication();
        }
    }

    static void DecimalBoost_CompoundDivision(benchmark::State& state) {
        for (auto _ : state) {
            CompoundDivision();
        }
    }

    static void DecimalBoost_Comparison(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Comparison());
        }
    }

    BENCHMARK(DecimalBoost_Addition);
    BENCHMARK(DecimalBoost_Subtraction);
    BENCHMARK(DecimalBoost_Multiplication);
    BENCHMARK(DecimalBoost_Division);
    BENCHMARK(DecimalBoost_CompoundAddition);
    BENCHMARK(DecimalBoost_CompoundSubtraction);
    BENCHMARK(DecimalBoost_CompoundMultiplication);
    BENCHMARK(DecimalBoost_CompoundDivision);
    BENCHMARK(DecimalBoost_Comparison);
}