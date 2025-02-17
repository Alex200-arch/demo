#include <benchmark/benchmark.h>
#include "../Decimal64.hpp"

namespace BM_Decimal64 {

    using Decimal_t = Decimal<8>;

    Decimal_t::UP Addition() {
        Decimal_t::UP a = Decimal_t::UP::fromRaw(25);
        Decimal_t::UP b = Decimal_t::UP::fromRaw(37);
        return a + b;
    }

    Decimal_t::DOWN Subtraction() {
        Decimal_t::DOWN a = Decimal_t::DOWN::fromRaw(58);
        Decimal_t::DOWN b = Decimal_t::DOWN::fromRaw(24);
        return a - b;
    }

    Decimal_t::UP Multiplication() {
        Decimal_t::UP a = Decimal_t::UP::fromRaw(15);
        Decimal_t::UP b = Decimal_t::UP::fromRaw(21);
        return a * b;
    }

    Decimal_t::UP DivisionUP() {
        Decimal_t::UP a = Decimal_t::UP::fromRaw(60);
        Decimal_t::UP b = Decimal_t::UP::fromRaw(20);
        return a / b;
    }

    Decimal_t::DOWN DivisionDOWN() {
        Decimal_t::DOWN a = Decimal_t::DOWN::fromRaw(60);
        Decimal_t::DOWN b = Decimal_t::DOWN::fromRaw(20);
        return a / b;
    }

    Decimal_t::NEAR DivisionNEAR() {
        Decimal_t::NEAR a = Decimal_t::NEAR::fromRaw(60);
        Decimal_t::NEAR b = Decimal_t::NEAR::fromRaw(20);
        return a / b;
    }

    void CompoundAddition() {
        Decimal_t::DOWN a = Decimal_t::DOWN::fromRaw(20);
        Decimal_t::DOWN b = Decimal_t::DOWN::fromRaw(30);
        a += b;
    }

    void CompoundSubtraction() {
        Decimal_t::NEAR a = Decimal_t::NEAR::fromRaw(50);
        Decimal_t::NEAR b = Decimal_t::NEAR::fromRaw(20);
        a -= b;
    }

    void CompoundMultiplication() {
        Decimal_t::UP a = Decimal_t::UP::fromRaw(20);
        Decimal_t::UP b = Decimal_t::UP::fromRaw(30);
        a *= b;
    }

    void CompoundDivisionUP() {
        Decimal_t::UP a = Decimal_t::UP::fromRaw(60);
        Decimal_t::UP b = Decimal_t::UP::fromRaw(20);
        a /= b;
    }

    void CompoundDivisionDOWN() {
        Decimal_t::DOWN a = Decimal_t::DOWN::fromRaw(60);
        Decimal_t::DOWN b = Decimal_t::DOWN::fromRaw(20);
        a /= b;
    }

    void CompoundDivisionNEAR() {
        Decimal_t::NEAR a = Decimal_t::NEAR::fromRaw(60);
        Decimal_t::NEAR b = Decimal_t::NEAR::fromRaw(20);
        a /= b;
    }

    bool Comparison() {
        Decimal_t::NEAR a = Decimal_t::NEAR::fromRaw(20);
        Decimal_t::NEAR b = Decimal_t::NEAR::fromRaw(30);
        bool r1 = a == b;
        bool r2 = a != b;
        bool r3 = a < b;
        bool r4 = a <= b;
        bool r5 = a > b;
        bool r6 = a >= b;
        return r1 && r2 && r3 && r4 && r5 && r6;
    }

    static void Decimal_Addition(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Addition());
        }
    }

    static void Decimal_Subtraction(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Subtraction());
        }
    }

    static void Decimal_Multiplication(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Multiplication());
        }
    }

    static void Decimal_DivisionUP(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(DivisionUP());
        }
    }

    static void Decimal_DivisionDOWN(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(DivisionDOWN());
        }
    }

    static void Decimal_DivisionNEAR(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(DivisionNEAR());
        }
    }

    static void Decimal_CompoundAddition(benchmark::State& state) {
        for (auto _ : state) {
            CompoundAddition();
        }
    }

    static void Decimal_CompoundSubtraction(benchmark::State& state) {
        for (auto _ : state) {
            CompoundSubtraction();
        }
    }

    static void Decimal_CompoundMultiplication(benchmark::State& state) {
        for (auto _ : state) {
            CompoundMultiplication();
        }
    }

    static void Decimal_CompoundDivisionUP(benchmark::State& state) {
        for (auto _ : state) {
            CompoundDivisionUP();
        }
    }

    static void Decimal_CompoundDivisionDOWN(benchmark::State& state) {
        for (auto _ : state) {
            CompoundDivisionDOWN();
        }
    }

    static void Decimal_CompoundDivisionNEAR(benchmark::State& state) {
        for (auto _ : state) {
            CompoundDivisionNEAR();
        }
    }

    static void Decimal_Comparison(benchmark::State& state) {
        for (auto _ : state) {
            benchmark::DoNotOptimize(Comparison());
        }
    }

    BENCHMARK(Decimal_Addition);
    BENCHMARK(Decimal_Subtraction);
    BENCHMARK(Decimal_Multiplication);
    BENCHMARK(Decimal_DivisionUP);
    BENCHMARK(Decimal_DivisionDOWN);
    BENCHMARK(Decimal_DivisionNEAR);
    BENCHMARK(Decimal_CompoundAddition);
    BENCHMARK(Decimal_CompoundSubtraction);
    BENCHMARK(Decimal_CompoundMultiplication);
    BENCHMARK(Decimal_CompoundDivisionUP);
    BENCHMARK(Decimal_CompoundDivisionDOWN);
    BENCHMARK(Decimal_CompoundDivisionNEAR);
    BENCHMARK(Decimal_Comparison);
}