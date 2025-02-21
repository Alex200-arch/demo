#ifndef __COMMON_LIB_Decimal64__
#define __COMMON_LIB_Decimal64__

#include <stdexcept>

template <uint N>
struct PowerOfTen {
    static constexpr int64_t value = 10 * PowerOfTen<N - 1>::value;
};
template <>
struct PowerOfTen<0> {
    static constexpr int64_t value = 1;
};

template <uint N>
struct DivideByTen {
    static constexpr double value = DivideByTen<N - 1>::value / 10.0;
};
template <>
struct DivideByTen<0> {
    static constexpr double value = 1.0;
};

template<uint DecimalPrecision>
class DecmailUp;

template<uint DecimalPrecision>
class DecmailDown;

template<uint DecimalPrecision>
class DecmailNear;

template<uint DecimalPrecision>
struct Decimal {
    typedef DecmailUp<DecimalPrecision> UP;
    typedef DecmailDown<DecimalPrecision> DOWN;
    typedef DecmailNear<DecimalPrecision> NEAR;
    static constexpr int64_t SCALING_FACTOR = PowerOfTen<DecimalPrecision>::value;
    static constexpr double ABS_ERROR = DivideByTen<DecimalPrecision>::value;
};

template<uint DecimalPrecision, typename Derived>
class DecimalBase {
protected:
    int64_t value;

    static void CheckAddOverflow(int64_t a, int64_t b) {
        if ((b > 0 && a > INT64_MAX - b) || 
            (b < 0 && a < INT64_MIN - b)) {
            throw std::overflow_error("Addition overflow");
        }
    }

    static void CheckSubOverflow(int64_t a, int64_t b) {
        if ((b > 0 && a < INT64_MIN + b) ||
            (b < 0 && a > INT64_MAX + b)) {
            throw std::overflow_error("Subtraction overflow");
        }
    }

    explicit DecimalBase(int64_t rawValue) : value(rawValue) {}

    DecimalBase() : value(0) {}

public:
    static Derived FromRaw(int64_t rawValue) {
        return Derived(rawValue);
    }

    static Derived FromInteger(int64_t integer) { 
        int64_t raw;
        if (__builtin_mul_overflow(integer, Decimal<DecimalPrecision>::SCALING_FACTOR, &raw)) {
            throw std::overflow_error("Integer conversion overflow");
        }
        return Derived(raw);
    }

    static Derived FromDouble(double d) {
        double scaled = d * Decimal<DecimalPrecision>::SCALING_FACTOR;
        if (scaled > INT64_MAX || scaled < INT64_MIN) {
            throw std::overflow_error("Double conversion overflow");
        }
        return Derived(static_cast<int64_t>(scaled));
    }

    double ToDouble() const {
        return static_cast<double>(value) / Decimal<DecimalPrecision>::SCALING_FACTOR;
    }

    int64_t GetRawValue() const {
        return value;
    }

    Derived operator+(const Derived& rhs) const {
        CheckAddOverflow(value, rhs.value);
        return Derived(value + rhs.value);
    }

    Derived operator-(const Derived& rhs) const {
        CheckSubOverflow(value, rhs.value);
        return Derived(value - rhs.value);
    }

    Derived operator*(const Derived& rhs) const {
        __int128_t result = static_cast<const Derived&>(*this).Multiply(rhs);
        if (result > INT64_MAX || result < INT64_MIN) {
            throw std::overflow_error("Multiplication overflow");
        }
        return Derived(static_cast<int64_t>(result));
    }

    Derived operator/(const Derived& rhs) const {
        if (rhs.value == 0) throw std::invalid_argument("Division by zero");
        __int128_t result = static_cast<const Derived&>(*this).Divide(rhs);
        if (result > INT64_MAX || result < INT64_MIN) {
            throw std::overflow_error("Division overflow");
        }
        return Derived(static_cast<int64_t>(result));
    }

    Derived& operator+=(const Derived& rhs) { 
        CheckAddOverflow(value, rhs.value);
        value += rhs.value;
        return static_cast<Derived&>(*this);
    }

    Derived& operator-=(const Derived& rhs) { 
        CheckSubOverflow(value, rhs.value);
        value -= rhs.value;
        return static_cast<Derived&>(*this);
    }

    Derived& operator*=(const Derived& rhs) {
        *this = *this * rhs;
        return static_cast<Derived&>(*this);
    }

    Derived& operator/=(const Derived& rhs) { 
        *this = *this / rhs;
        return static_cast<Derived&>(*this);
    }

    bool operator==(const Derived& rhs) const { return value == rhs.value; }
    bool operator!=(const Derived& rhs) const { return value != rhs.value; }
    bool operator<(const Derived& rhs) const { return value < rhs.value; }
    bool operator<=(const Derived& rhs) const { return value <= rhs.value; }
    bool operator>(const Derived& rhs) const { return value > rhs.value; }
    bool operator>=(const Derived& rhs) const { return value >= rhs.value; }
};

template<uint DecimalPrecision>
class DecmailUp : public DecimalBase<DecimalPrecision, DecmailUp<DecimalPrecision>> {
public:
    explicit DecmailUp(int64_t rawValue) : DecimalBase<DecimalPrecision, DecmailUp>(rawValue) {}

    DecmailUp() : DecimalBase<DecimalPrecision, DecmailUp>() {}

    __int128_t Multiply(const DecmailUp& rhs) const {
        static constexpr int64_t SCALING_FACTOR_DIV10 = Decimal<DecimalPrecision>::SCALING_FACTOR / 10;
        const __int128_t product = static_cast<__int128_t>((*this).value) * rhs.value;
        const __int128_t adjusted = product + (product > 0 ? (Decimal<DecimalPrecision>::SCALING_FACTOR - SCALING_FACTOR_DIV10) : 
                                                SCALING_FACTOR_DIV10 - Decimal<DecimalPrecision>::SCALING_FACTOR);
        return adjusted / Decimal<DecimalPrecision>::SCALING_FACTOR;
    }

    __int128_t Divide(const DecmailUp& rhs) const {
        const bool same_sign = ((*this).value > 0) == (rhs.value > 0);
        const __int128_t dividend = static_cast<__int128_t>((*this).value) * Decimal<DecimalPrecision>::SCALING_FACTOR;
        const __int128_t divsor = rhs.value;
        const __int128_t adjusted = (10 * dividend) + (same_sign > 0 ? (9 * divsor) : -(9 * divsor));
        return adjusted / (10 * divsor);
    }
};

template<>
class DecmailUp<0> : public DecimalBase<0, DecmailUp<0>> {
public:
    explicit DecmailUp(int64_t rawValue) : DecimalBase<0, DecmailUp>(rawValue) {}

    DecmailUp() : DecimalBase<0, DecmailUp>() {}

    __int128_t Multiply(const DecmailUp& rhs) const {
        return static_cast<__int128_t>((*this).value) * rhs.value;
    }

    __int128_t Divide(const DecmailUp& rhs) const {
        const bool same_sign = ((*this).value > 0) == (rhs.value > 0);
        const __int128_t dividend = static_cast<__int128_t>((*this).value);
        const __int128_t divsor = rhs.value;
        const __int128_t adjusted = (10 * dividend) + (same_sign > 0 ? (9*divsor) : -(9*divsor));
        return adjusted / (10 * divsor);
    }
};

template<uint DecimalPrecision>
class DecmailDown : public DecimalBase<DecimalPrecision, DecmailDown<DecimalPrecision>> {
public:
    explicit DecmailDown(int64_t rawValue) : DecimalBase<DecimalPrecision, DecmailDown>(rawValue) {}

    DecmailDown() : DecimalBase<DecimalPrecision, DecmailDown>() {}

    __int128_t Multiply(const DecmailDown& rhs) const {
        const __int128_t product = static_cast<__int128_t>((*this).value) * rhs.value;
        return product / Decimal<DecimalPrecision>::SCALING_FACTOR;
    }

    __int128_t Divide(const DecmailDown& rhs) const {
        const __int128_t dividend = static_cast<__int128_t>((*this).value) * Decimal<DecimalPrecision>::SCALING_FACTOR;
        return dividend / rhs.value;
    }
};

template<uint DecimalPrecision>
class DecmailNear : public DecimalBase<DecimalPrecision, DecmailNear<DecimalPrecision>> {
public:
    explicit DecmailNear(int64_t rawValue) : DecimalBase<DecimalPrecision, DecmailNear>(rawValue) {}

    DecmailNear() : DecimalBase<DecimalPrecision, DecmailNear>() {}

    __int128_t Multiply(const DecmailNear& rhs) const {
        static constexpr int64_t HALF_FACTOR = Decimal<DecimalPrecision>::SCALING_FACTOR / 2;
        const __int128_t product = static_cast<__int128_t>((*this).value) * rhs.value;
        const __int128_t adjusted = product + (product > 0 ? HALF_FACTOR : -HALF_FACTOR);
        return adjusted / Decimal<DecimalPrecision>::SCALING_FACTOR;
    }

    __int128_t Divide(const DecmailNear& rhs) const {
        const bool same_sign = ((*this).value > 0) == (rhs.value > 0);
        const __int128_t dividend = static_cast<__int128_t>((*this).value) * Decimal<DecimalPrecision>::SCALING_FACTOR;
        const __int128_t divsor = rhs.value;
        const __int128_t adjusted = (10 * dividend) + (same_sign > 0 ? (5 * divsor) : -(5 * divsor));
        return adjusted / (10 * divsor);
    }
};

template<>
class DecmailNear<0> : public DecimalBase<0, DecmailNear<0>> {
public:
    explicit DecmailNear(int64_t rawValue) : DecimalBase<0, DecmailNear>(rawValue) {}

    DecmailNear() : DecimalBase<0, DecmailNear>() {}

    __int128_t Multiply(const DecmailNear& rhs) const {
        return static_cast<__int128_t>((*this).value) * rhs.value;
    }

    __int128_t Divide(const DecmailNear& rhs) const {
        const bool same_sign = ((*this).value > 0) == (rhs.value > 0);
        const __int128_t dividend = static_cast<__int128_t>((*this).value);
        const __int128_t divsor = rhs.value;
        const __int128_t adjusted = (10 * dividend) + (same_sign > 0 ? (5 * divsor) : -(5 * divsor));
        return adjusted / (10 * divsor);
    }
};

#endif

