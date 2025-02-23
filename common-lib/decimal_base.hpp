#ifndef __COMMON_LIB_Decimal_BASE__
#define __COMMON_LIB_Decimal_BASE__

#include <iomanip>

template <uint N>
struct PowerOfTen {
    static constexpr int64_t value = 10 * PowerOfTen<N - 1>::value;
};
template <>
struct PowerOfTen<0> {
    static constexpr int64_t value = 1;
};

/*
    ShortDecimal and LongDecimal
    Basic Function
        comparison: ==, !=, <, <=, >, >=
        arithmetic: +, +=, -, -=
*/
template<uint Precision, typename Derived>
class ShortDecimal {
protected:
    static_assert(Precision >= 0 && Precision <= 9, "Precision must be between 0 and 9.");
    static constexpr int64_t SCALING_FACTOR = PowerOfTen<Precision>::value;
    int64_t value = 0;

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

    explicit ShortDecimal(int64_t raw_value) : value(raw_value) {}

public:
    static Derived FromRaw(int64_t raw_value) {
        return Derived(raw_value);
    }

    static Derived FromInteger(int64_t integer) { 
        int64_t raw_value;
        if (__builtin_mul_overflow(integer, SCALING_FACTOR, &raw_value)) {
            throw std::overflow_error("Integer conversion overflow");
        }
        return Derived(raw_value);
    }

    static Derived FromDouble(double d) {
        double raw_value = d * SCALING_FACTOR;
        if (raw_value > INT64_MAX || raw_value < INT64_MIN) {
            throw std::overflow_error("Double conversion overflow");
        }
        return Derived(static_cast<int64_t>(raw_value));
    }

    static Derived FromString(const std::string& str) {
        bool negative = false;
        size_t start = 0;
        if (!str.empty()) {
            if (str[0] == '+') {
                start = 1;
            } else if (str[0] == '-') {
                negative = true;
                start = 1;
            }
        }

        size_t dot_pos = str.find('.', start);
        std::string int_part_str;
        std::string dec_part_str;

        if (dot_pos == std::string::npos) {
            int_part_str = str.substr(start);
            dec_part_str = "";
        } else {
            int_part_str = str.substr(start, dot_pos - start);
            dec_part_str = str.substr(dot_pos + 1);
        }

        if (int_part_str.empty()) {
            int_part_str = "0";
        }

        if (dec_part_str.length() > Precision) {
            throw std::invalid_argument("Decimal part exceeds precision");
        }

        if (dec_part_str.length() < Precision) {
            dec_part_str.append(Precision - dec_part_str.length(), '0');
        }

        auto is_digits = [](const std::string& s) {
            return s.empty() || std::all_of(s.begin(), s.end(), ::isdigit);
        };

        if (!is_digits(int_part_str)) {
            throw std::invalid_argument("Invalid integer part");
        }
        if (!is_digits(dec_part_str)) {
            throw std::invalid_argument("Invalid decimal part");
        }

        int64_t int_val = 0;
        static constexpr int64_t INT64_MAX_DEV10 = INT64_MAX / 10;
        for (const char& c : int_part_str) {
            if (int_val > INT64_MAX_DEV10) {
                throw std::overflow_error("Integer part overflow");
            }
            int_val *= 10;
            int digit = c - '0';
            if (int_val > INT64_MAX - digit) {
                throw std::overflow_error("Integer part overflow");
            }
            int_val += digit;
        }

        int64_t dec_val = dec_part_str.empty() ? 0 : std::stoll(dec_part_str);

        int64_t scaled_int;
        if (__builtin_mul_overflow(int_val, SCALING_FACTOR, &scaled_int)) {
            throw std::overflow_error("Overflow in scaling integer part");
        }

        int64_t raw_value;
        if (__builtin_add_overflow(scaled_int, dec_val, &raw_value)) {
            throw std::overflow_error("Overflow in adding decimal part");
        }

        return Derived(negative ? -raw_value : raw_value);
    }

    int64_t GetRaw() const {
        return value;
    }

    double ToDouble() const {
        return static_cast<double>(value) / SCALING_FACTOR;
    }

    std::string ToString() const {
        bool negative = (value < 0);
        uint64_t abs_raw = value >= 0 ? value : -value;
        uint64_t int_part = abs_raw / SCALING_FACTOR;
        uint64_t dec_part = abs_raw % SCALING_FACTOR;

        std::ostringstream oss;
        if (negative) {
            oss << "-";
        }
        oss << int_part;
        if constexpr (Precision > 0) {
            oss << "."
                << std::setw(Precision) << std::setfill('0') << dec_part;
        }

        return oss.str();
    }

    Derived operator+(const Derived& rhs) const {
        CheckAddOverflow(value, rhs.value);
        return Derived(value + rhs.value);
    }

    Derived operator-(const Derived& rhs) const {
        CheckSubOverflow(value, rhs.value);
        return Derived(value - rhs.value);
    }

    Derived operator*(const int64_t k) const {
        int64_t result;
        if (__builtin_mul_overflow(value, k, &result)) {
            throw std::overflow_error("Derived multiplication overflow");
        }
        return Derived(result);
    }

    friend Derived operator*(const int64_t k, const Derived& sd) {
        return sd * k;
    }

    Derived& operator+=(const Derived& rhs) { 
        CheckAddOverflow(value, rhs.value);
        value += rhs.value;
        return static_cast<Derived&>(*this);;
    }

    Derived& operator-=(const Derived& rhs) { 
        CheckSubOverflow(value, rhs.value);
        value -= rhs.value;
        return static_cast<Derived&>(*this);;
    }

    bool operator==(const Derived& rhs) const { return value == rhs.value; }
    bool operator!=(const Derived& rhs) const { return value != rhs.value; }
    bool operator<(const Derived& rhs) const { return value < rhs.value; }
    bool operator<=(const Derived& rhs) const { return value <= rhs.value; }
    bool operator>(const Derived& rhs) const { return value > rhs.value; }
    bool operator>=(const Derived& rhs) const { return value >= rhs.value; }
};

template <int Precision>
class LongDecimal {
private:
    struct internal_t {
        int64_t integer;
        int64_t decimal;
    };
public:
    typedef internal_t raw_type;
private:
    static_assert(Precision >= 0 && Precision <= 18, "Precision must be between 0 and 18.");
    static constexpr int64_t FACTOR = PowerOfTen<Precision>::value;
    raw_type value;

    explicit LongDecimal(raw_type raw_value)
        : value(raw_value) {
        if (value.decimal < 0 || value.decimal >= FACTOR) {
            throw std::overflow_error("Decimal part overflow");
        }
    }

public:
    static LongDecimal FromRaw(int64_t integer_part, int64_t decimal_part) {
        return LongDecimal({integer_part, decimal_part});
    }

    static LongDecimal FromRaw(raw_type raw_value) {
        return LongDecimal(raw_value);
    }

    static LongDecimal FromInteger(int64_t value) {
        return LongDecimal({value, 0});
    }

    static LongDecimal FromDouble(double value) {
        static constexpr __int128_t INT128_MAX = (static_cast<__int128_t>(0x7fffffffffffffff) << 64) | 0xffffffffffffffff;
        static constexpr __int128_t INT128_MIN = (__int128(1) << 127);
        double scaled = value * FACTOR;
        if (scaled > INT128_MAX || scaled < INT128_MIN) {
            throw std::overflow_error("Double conversion overflow");
        }
        __int128_t total = static_cast<__int128_t>(scaled);
        __int128_t integer_128 = total / FACTOR;
        __int128_t decimal_128 = total % FACTOR;
        if (integer_128 > INT64_MAX || integer_128 < INT64_MIN) {
            throw std::overflow_error("Double conversion overflow");
        }
        int64_t integer = static_cast<int64_t>(integer_128);
        int64_t decimal = static_cast<int64_t>(decimal_128);
        return LongDecimal({integer, decimal >= 0 ? decimal : -decimal});
    }

    static LongDecimal FromString(const std::string& str) {
        bool negative = false;
        size_t start = 0;
        if (!str.empty()) {
            if (str[0] == '+') {
                start = 1;
            } else if (str[0] == '-') {
                negative = true;
                start = 1;
            }
        }

        size_t dot_pos = str.find('.', start);
        std::string int_part_str;
        std::string dec_part_str;

        if (dot_pos == std::string::npos) {
            int_part_str = str.substr(start);
            dec_part_str = "";
        } else {
            int_part_str = str.substr(start, dot_pos - start);
            dec_part_str = str.substr(dot_pos + 1);
        }

        if (int_part_str.empty()) {
            int_part_str = "0";
        }

        if (dec_part_str.length() > Precision) {
            throw std::invalid_argument("Decimal part exceeds precision");
        }

        if (dec_part_str.length() < Precision) {
            dec_part_str.append(Precision - dec_part_str.length(), '0');
        }

        auto is_digits = [](const std::string& s) {
            return s.empty() || std::all_of(s.begin(), s.end(), ::isdigit);
        };

        if (!is_digits(int_part_str)) {
            throw std::invalid_argument("Invalid integer part");
        }
        if (!is_digits(dec_part_str)) {
            throw std::invalid_argument("Invalid decimal part");
        }

        int64_t int_val = 0;
        static constexpr int64_t INT64_MAX_DEV10 = INT64_MAX / 10;
        for (const char& c : int_part_str) {
            if (int_val > INT64_MAX_DEV10) {
                throw std::overflow_error("Integer part overflow");
            }
            int_val *= 10;
            int digit = c - '0';
            if (int_val > INT64_MAX - digit) {
                throw std::overflow_error("Integer part overflow");
            }
            int_val += digit;
        }

        int64_t dec_val = dec_part_str.empty() ? 0 : std::stoll(dec_part_str);

        return LongDecimal({negative ? -int_val : int_val, dec_val});
    }

    raw_type GetRaw() const {
        return value;
    }

    double ToDouble() const {
        return value.integer + static_cast<double>(value.decimal) / FACTOR;
    }

    std::string ToString() const {
        std::stringstream oss;
        oss << value.integer;
        if constexpr (Precision > 0) {
            oss << "."
                << std::setw(Precision) << std::setfill('0') << value.decimal;
        }
        return oss.str();
    }

    LongDecimal operator+(const LongDecimal& rhs) const {
        int64_t sum_int;
        if (__builtin_add_overflow(value.integer, rhs.value.integer, &sum_int)) {
            throw std::overflow_error("Integer addition overflow");
        }

        int64_t sum_dec = value.decimal + rhs.value.decimal;
        int64_t carry = sum_dec / FACTOR;
        sum_dec %= FACTOR;

        if (carry > 0) {
            if (__builtin_add_overflow(sum_int, carry, &sum_int)) {
                throw std::overflow_error("Carry overflow in addition");
            }
        }

        return LongDecimal({sum_int, sum_dec});
    }

    LongDecimal operator-(const LongDecimal& rhs) const {
        int64_t diff_int;
        if (__builtin_sub_overflow(value.integer, rhs.value.integer, &diff_int)) {
            throw std::overflow_error("Integer subtraction overflow");
        }

        int64_t diff_dec = value.decimal - rhs.value.decimal;
        if (diff_dec < 0) {
            diff_dec += FACTOR;
            if (__builtin_sub_overflow(diff_int, 1, &diff_int)) {
                throw std::overflow_error("Borrow overflow in subtraction");
            }
        }

        return LongDecimal({diff_int, diff_dec});
    }

    LongDecimal operator*(int64_t k) const {
        if constexpr (Precision == 0) {
            int64_t new_integer;
            if (__builtin_mul_overflow(value.integer, k, &new_integer)) {
                throw std::overflow_error("LongDecimal integer multiplication overflow");
            }
            return LongDecimal({new_integer, 0});
        } else {
            int64_t new_integer;
            if (__builtin_mul_overflow(value.integer, k, &new_integer)) {
                throw std::overflow_error("LongDecimal integer multiplication overflow");
            }

            k = k >= 0 ? k : -k;
            __int128_t decimal_product = static_cast<__int128_t>(value.decimal) * k;
            __int128_t carry = decimal_product / FACTOR;
            __int128_t new_decimal = decimal_product % FACTOR;

            if (carry != 0) {
                int64_t carry64 = static_cast<int64_t>(carry);
                if ((new_integer > 0 && new_integer > INT64_MAX - carry64) ||
                    (new_integer < 0 && new_integer < INT64_MIN + carry64)) {
                    throw std::overflow_error("LongDecimal carry overflow");
                }
                new_integer += (new_integer >= 0) ? carry64 : -carry64;
            }

            return LongDecimal({new_integer, static_cast<int64_t>(new_decimal)});
        }
    }

    friend LongDecimal operator*(const int64_t k, const LongDecimal& ld) {
        return ld * k;
    }

    LongDecimal& operator+=(const LongDecimal& rhs) {
        *this = *this + rhs;
        return *this;
    }

    LongDecimal& operator-=(const LongDecimal& rhs) {
        *this = *this - rhs;
        return *this;
    }

    bool operator==(const LongDecimal& rhs) const { 
        return value.integer == rhs.value.integer && value.decimal == rhs.value.decimal;
    }
    bool operator!=(const LongDecimal& rhs) const { return !(*this == rhs); }
    bool operator<(const LongDecimal& rhs) const {
        if (value.integer != rhs.value.integer) {
            return value.integer < rhs.value.integer;
        }
        return value.decimal < rhs.value.decimal;
    }
    bool operator<=(const LongDecimal& rhs) const { return *this < rhs || *this == rhs; }
    bool operator>(const LongDecimal& rhs) const { return !(*this <= rhs); }
    bool operator>=(const LongDecimal& rhs) const { return !(*this < rhs); }
};

enum class RoundModel {
    UP, DOWN, NEAR
};

template<uint P>
class ShortQuantity : public ShortDecimal<P, ShortQuantity<P>> {
public:
    explicit ShortQuantity(int64_t raw_value) : ShortDecimal<P, ShortQuantity>(raw_value) {}
};

template<uint P>
class Price : public ShortDecimal<P, Price<P>> {
public:
    explicit Price(int64_t raw_value) : ShortDecimal<P, Price>(raw_value) {}

    template<uint SP, RoundModel RM>
    friend Price operator*(const Price& px, const ShortQuantity<SP>& qty) {
        int64_t price_raw = px.GetRaw();
        int64_t qty_raw = qty.GetRaw();

        __int128_t product = static_cast<__int128_t>(price_raw) * static_cast<__int128_t>(qty_raw);

        if constexpr (SP > 0) {
            if constexpr (RM == RoundModel::UP) {
                static constexpr __int128_t adjust = PowerOfTen<SP>::value - PowerOfTen<SP - 1>::value;
                product += (product > 0) ? adjust : -adjust;
            } else if constexpr (RM == RoundModel::NEAR) {
                static constexpr __int128_t adjust = PowerOfTen<SP>::value / 2;
                product += (product > 0) ? adjust : -adjust;
            }

            product /= PowerOfTen<SP>::value;
        }
        
        if (product > INT64_MAX || product < INT64_MIN) {
            throw std::overflow_error("Multiplication result overflow");
        }
    
        return Price::FromRaw(static_cast<int64_t>(product));
    }

    template<uint SP, RoundModel RM>
    friend Price operator*(const ShortQuantity<SP>& qty, const Price& px) {
        return px * qty;
    }
};


#endif
