#include <benchmark/benchmark.h>
#include "../decimal64.hpp"

namespace BM_Decimal64 {

    using Decimal_T = Decimal<8>;

    Decimal_T::UP Addition() {
        Decimal_T::UP a = Decimal_T::UP::FromRaw(25);
        Decimal_T::UP b = Decimal_T::UP::FromRaw(37);
        return a + b;
    }

    Decimal_T::DOWN Subtraction() {
        Decimal_T::DOWN a = Decimal_T::DOWN::FromRaw(58);
        Decimal_T::DOWN b = Decimal_T::DOWN::FromRaw(24);
        return a - b;
    }

    Decimal_T::UP Multiplication() {
        Decimal_T::UP a = Decimal_T::UP::FromRaw(15);
        Decimal_T::UP b = Decimal_T::UP::FromRaw(21);
        return a * b;
    }

    Decimal_T::UP DivisionUP() {
        Decimal_T::UP a = Decimal_T::UP::FromRaw(60);
        Decimal_T::UP b = Decimal_T::UP::FromRaw(20);
        return a / b;
    }

    Decimal_T::DOWN DivisionDOWN() {
        Decimal_T::DOWN a = Decimal_T::DOWN::FromRaw(60);
        Decimal_T::DOWN b = Decimal_T::DOWN::FromRaw(20);
        return a / b;
    }

    Decimal_T::NEAR DivisionNEAR() {
        Decimal_T::NEAR a = Decimal_T::NEAR::FromRaw(60);
        Decimal_T::NEAR b = Decimal_T::NEAR::FromRaw(20);
        return a / b;
    }

    void CompoundAddition() {
        Decimal_T::DOWN a = Decimal_T::DOWN::FromRaw(20);
        Decimal_T::DOWN b = Decimal_T::DOWN::FromRaw(30);
        a += b;
    }

    void CompoundSubtraction() {
        Decimal_T::NEAR a = Decimal_T::NEAR::FromRaw(50);
        Decimal_T::NEAR b = Decimal_T::NEAR::FromRaw(20);
        a -= b;
    }

    void CompoundMultiplication() {
        Decimal_T::UP a = Decimal_T::UP::FromRaw(20);
        Decimal_T::UP b = Decimal_T::UP::FromRaw(30);
        a *= b;
    }

    void CompoundDivisionUP() {
        Decimal_T::UP a = Decimal_T::UP::FromRaw(60);
        Decimal_T::UP b = Decimal_T::UP::FromRaw(20);
        a /= b;
    }

    void CompoundDivisionDOWN() {
        Decimal_T::DOWN a = Decimal_T::DOWN::FromRaw(60);
        Decimal_T::DOWN b = Decimal_T::DOWN::FromRaw(20);
        a /= b;
    }

    void CompoundDivisionNEAR() {
        Decimal_T::NEAR a = Decimal_T::NEAR::FromRaw(60);
        Decimal_T::NEAR b = Decimal_T::NEAR::FromRaw(20);
        a /= b;
    }

    bool Comparison() {
        Decimal_T::NEAR a = Decimal_T::NEAR::FromRaw(20);
        Decimal_T::NEAR b = Decimal_T::NEAR::FromRaw(30);
        bool r1 = a == b;
        bool r2 = a != b;
        bool r3 = a < b;
        bool r4 = a <= b;
        bool r5 = a > b;
        bool r6 = a >= b;
        return r1 && r2 && r3 && r4 && r5 && r6;
    }

    static void DecimalAddition(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Addition());
        }
    }

    static void DecimalSubtraction(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Subtraction());
        }
    }

    static void DecimalMultiplication(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Multiplication());
        }
    }

    static void DecimalDivisionUP(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(DivisionUP());
        }
    }

    static void DecimalDivisionDOWN(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(DivisionDOWN());
        }
    }

    static void DecimalDivisionNEAR(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(DivisionNEAR());
        }
    }

    static void DecimalCompoundAddition(benchmark::State& state) {
        for (auto _ : state) {
            CompoundAddition();
        }
    }

    static void DecimalCompoundSubtraction(benchmark::State& state) {
        for (auto _ : state) {
            CompoundSubtraction();
        }
    }

    static void DecimalCompoundMultiplication(benchmark::State& state) {
        for (auto _ : state) {
            CompoundMultiplication();
        }
    }

    static void DecimalCompoundDivisionUP(benchmark::State& state) {
        for (auto _ : state) {
            CompoundDivisionUP();
        }
    }

    static void DecimalCompoundDivisionDOWN(benchmark::State& state) {
        for (auto _ : state) {
            CompoundDivisionDOWN();
        }
    }

    static void DecimalCompoundDivisionNEAR(benchmark::State& state) {
        for (auto _ : state) {
            CompoundDivisionNEAR();
        }
    }

    static void DecimalComparison(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Comparison());
        }
    }

    BENCHMARK(DecimalAddition);
    BENCHMARK(DecimalSubtraction);
    BENCHMARK(DecimalMultiplication);
    BENCHMARK(DecimalDivisionUP);
    BENCHMARK(DecimalDivisionDOWN);
    BENCHMARK(DecimalDivisionNEAR);
    BENCHMARK(DecimalCompoundAddition);
    BENCHMARK(DecimalCompoundSubtraction);
    BENCHMARK(DecimalCompoundMultiplication);
    BENCHMARK(DecimalCompoundDivisionUP);
    BENCHMARK(DecimalCompoundDivisionDOWN);
    BENCHMARK(DecimalCompoundDivisionNEAR);
    BENCHMARK(DecimalComparison);
}