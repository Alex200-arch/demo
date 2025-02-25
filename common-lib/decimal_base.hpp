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

struct LongDecimalRaw {
    int64_t integer;
    int64_t decimal;
};

// round tags
struct RoundModel {
    struct Up {};
    struct Down {};
    struct Near {};
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
        uint64_t abs_raw = value >= 0 ? value : -value;
        uint64_t int_part = abs_raw / SCALING_FACTOR;
        uint64_t dec_part = abs_raw % SCALING_FACTOR;

        std::ostringstream oss;
        if (value < 0) {
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

    Derived DivInt(const int64_t k, RoundModel::Up) const {
        if (k == 0) {
            throw std::invalid_argument("Division by zero");
        }

        const bool same_sign = (value > 0) == (k > 0);
        const __int128_t dividend = static_cast<__int128_t>(value);
        const __int128_t adjusted = (10 * dividend) + (same_sign > 0 ? (9*k) : -(9*k));
        __int128_t result =  adjusted / (10 * k);

        if (result > INT64_MAX || result < INT64_MIN) {
            throw std::overflow_error("Division overflow");
        }
        return Derived(static_cast<int64_t>(result));
    }

    Derived DivInt(const int64_t k, RoundModel::Down) const {
        if (k == 0) {
            throw std::invalid_argument("Division by zero");
        }

        const __int128_t dividend = static_cast<__int128_t>(value);
        __int128_t result = dividend / k;

        if (result > INT64_MAX || result < INT64_MIN) {
            throw std::overflow_error("Division overflow");
        }
        return Derived(static_cast<int64_t>(result));
    }

    Derived DivInt(const int64_t k, RoundModel::Near) const {
        if (k == 0) {
            throw std::invalid_argument("Division by zero");
        }

        const bool same_sign = (value > 0) == (k > 0);
        const __int128_t dividend = static_cast<__int128_t>(value);
        const __int128_t adjusted = (10 * dividend) + (same_sign > 0 ? (5*k) : -(5*k));
        __int128_t result =  adjusted / (10 * k);

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

    bool operator==(const Derived& rhs) const { return value == rhs.value; }
    bool operator!=(const Derived& rhs) const { return value != rhs.value; }
    bool operator<(const Derived& rhs) const { return value < rhs.value; }
    bool operator<=(const Derived& rhs) const { return value <= rhs.value; }
    bool operator>(const Derived& rhs) const { return value > rhs.value; }
    bool operator>=(const Derived& rhs) const { return value >= rhs.value; }
};

template <int Precision, typename Derived>
class LongDecimal {
protected:
    static_assert(Precision >= 0 && Precision <= 18, "Precision must be between 0 and 18.");
    static constexpr int64_t FACTOR = PowerOfTen<Precision>::value;
    LongDecimalRaw value;

    bool Negative() const {
        return (value.integer < 0) || (value.integer == 0 && value.decimal < 0);
    }

    explicit LongDecimal(LongDecimalRaw raw_value)
        : value(raw_value) {
        if (value.decimal <= -FACTOR || value.decimal >= FACTOR) {
            throw std::overflow_error("Decimal part overflow");
        }
        if ((value.integer != 0) && (value.decimal != 0)) {
            if (!(value.integer > 0) == (value.decimal > 0)) {
                throw std::invalid_argument("Positive or negative signs are not same");
            }
        }
    }

public:
    static Derived FromRaw(int64_t integer_part, int64_t decimal_part) {
        return Derived({integer_part, decimal_part});
    }

    static Derived FromRaw(LongDecimalRaw raw_value) {
        return Derived(raw_value);
    }

    static Derived FromInteger(int64_t value) {
        return Derived({value, 0});
    }

    static Derived FromDouble(double value) {
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
        return Derived({integer, decimal});
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

        return Derived({negative ? -int_val : int_val, negative ? -dec_val : dec_val});
    }

    LongDecimalRaw GetRaw() const {
        return value;
    }

    double ToDouble() const {
        return value.integer + static_cast<double>(value.decimal) / FACTOR;
    }

    std::string ToString() const {
        bool negative = Negative();
        uint64_t abs_integer = value.integer >= 0 ? value.integer : -value.integer;
        uint64_t abs_decimal = value.decimal >= 0 ? value.decimal : -value.decimal;
    
        std::ostringstream oss;
        if (negative) {
            oss << "-";
        }
        oss << abs_integer;
        if constexpr (Precision > 0) {
            oss << "."
                << std::setw(Precision) << std::setfill('0') 
                << abs_decimal;
        }
        return oss.str();
    }

    Derived operator+(const Derived& rhs) const {
        int64_t sum_int;
        if (__builtin_add_overflow(value.integer, rhs.value.integer, &sum_int)) {
            throw std::overflow_error("Integer addition overflow");
        }
    
        int64_t sum_dec = value.decimal + rhs.value.decimal;
        int64_t carry = sum_dec / FACTOR;
        sum_dec %= FACTOR;
    
        if (carry != 0) {
            if (__builtin_add_overflow(sum_int, carry, &sum_int)) {
                throw std::overflow_error("Carry overflow in addition");
            }
        }
    
        if (sum_int != 0 && sum_dec != 0) {
            if ((sum_int > 0) != (sum_dec > 0)) {
                if (sum_int > 0) {
                    sum_int -= 1;
                    sum_dec += FACTOR;
                } else {
                    sum_int += 1;
                    sum_dec -= FACTOR;
                }
            }
        }
    
        return Derived({sum_int, sum_dec});
    }

    Derived operator-(const Derived& rhs) const {
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
    
        if (diff_int != 0 && diff_dec != 0) {
            if ((diff_int > 0) != (diff_dec > 0)) {
                if (diff_int > 0) {
                    diff_int -= 1;
                    diff_dec += FACTOR;
                } else {
                    diff_int += 1;
                    diff_dec -= FACTOR;
                }
            }
        }
    
        return Derived({diff_int, diff_dec});
    }

    Derived operator*(int64_t k) const {
        if constexpr (Precision == 0) {
            int64_t new_integer;
            if (__builtin_mul_overflow(value.integer, k, &new_integer)) {
                throw std::overflow_error("Integer multiplication overflow");
            }
            return Derived({new_integer, 0});
        } else {
            int64_t new_integer;
            if (__builtin_mul_overflow(value.integer, k, &new_integer)) {
                throw std::overflow_error("Integer multiplication overflow");
            }
    
            __int128_t decimal_product = static_cast<__int128_t>(value.decimal) * k;
            __int128_t carry = decimal_product / FACTOR;
            __int128_t new_decimal = decimal_product % FACTOR;
    
            if (carry != 0) {
                int64_t carry64 = static_cast<int64_t>(carry);
                if (__builtin_add_overflow(new_integer, carry64, &new_integer)) {
                    throw std::overflow_error("Carry overflow in multiplication");
                }
            }
    
            return Derived({new_integer, static_cast<int64_t>(new_decimal)});
        }
    }

    friend Derived operator*(const int64_t k, const Derived& ld) {
        return ld * k;
    }

    Derived DivInt(const int64_t k, RoundModel::Up) const {
        if (k == 0) {
            throw std::invalid_argument("Division by zero");
        }

        const bool same_sign = Negative() == (k < 0);
        const __int128_t dividend = static_cast<__int128_t>(value.integer) * FACTOR + value.decimal;
        const __int128_t adjusted = (10 * dividend) + (same_sign > 0 ? (9*k) : -(9*k));
        __int128_t result = adjusted / (10 * k);

        __int128_t integer = result / FACTOR;
        __int128_t decimal = result % FACTOR;

        return Derived({static_cast<int64_t>(integer), static_cast<int64_t>(decimal)});
    }

    Derived DivInt(const int64_t k, RoundModel::Down) const {
        if (k == 0) {
            throw std::invalid_argument("Division by zero");
        }

        const __int128_t dividend = static_cast<__int128_t>(value.integer) * FACTOR + value.decimal;
        __int128_t result = dividend / k;

        __int128_t integer = result / FACTOR;
        __int128_t decimal = result % FACTOR;

        return Derived({static_cast<int64_t>(integer), static_cast<int64_t>(decimal)});
    }

    Derived DivInt(const int64_t k, RoundModel::Near) const {
        if (k == 0) {
            throw std::invalid_argument("Division by zero");
        }

        const bool same_sign = Negative() == (k < 0);
        const __int128_t dividend = static_cast<__int128_t>(value.integer) * FACTOR + value.decimal;
        const __int128_t adjusted = (10 * dividend) + (same_sign > 0 ? (5*k) : -(5*k));
        __int128_t result =  adjusted / (10 * k);

        __int128_t integer = result / FACTOR;
        __int128_t decimal = result % FACTOR;

        return Derived({static_cast<int64_t>(integer), static_cast<int64_t>(decimal)});
    }

    Derived& operator+=(const Derived& rhs) {
        *this = *this + rhs;
        return static_cast<Derived&>(*this);
    }

    Derived& operator-=(const Derived& rhs) {
        *this = *this - rhs;
        return static_cast<Derived&>(*this);
    }

    bool operator==(const Derived& rhs) const { 
        return value.integer == rhs.value.integer && value.decimal == rhs.value.decimal;
    }
    bool operator!=(const Derived& rhs) const { return !(*this == rhs); }
    bool operator<(const Derived& rhs) const {
        if (value.integer != rhs.value.integer) {
            return value.integer < rhs.value.integer;
        }
        return value.decimal < rhs.value.decimal;
    }
    bool operator<=(const Derived& rhs) const { return *this < rhs || *this == rhs; }
    bool operator>(const Derived& rhs) const { return !(*this <= rhs); }
    bool operator>=(const Derived& rhs) const { return !(*this < rhs); }
};

template<uint P>
class Price : public ShortDecimal<P, Price<P>> {
public:
    explicit Price(int64_t raw_value) : ShortDecimal<P, Price>(raw_value) {}
};

template<uint P>
class ShortQuantity : public ShortDecimal<P, ShortQuantity<P>> {
public:
    explicit ShortQuantity(int64_t raw_value) : ShortDecimal<P, ShortQuantity>(raw_value) {}
};

template<uint P>
class LongQuantity : public LongDecimal<P, LongQuantity<P>> {
public:
    explicit LongQuantity(LongDecimalRaw raw_value) : LongDecimal<P, LongQuantity>(raw_value) {}
};

template<uint PP, uint SP>
Price<PP> PxMulQty(const Price<PP>& px, const ShortQuantity<SP>& qty, RoundModel::Up) {
    int64_t price_raw = px.GetRaw();
    int64_t qty_raw = qty.GetRaw();

    __int128_t product = static_cast<__int128_t>(price_raw) * static_cast<__int128_t>(qty_raw);

    if constexpr (SP > 0) {
        static constexpr __int128_t adjust = PowerOfTen<SP>::value - PowerOfTen<SP - 1>::value;
        product += (product > 0) ? adjust : -adjust;
        product /= PowerOfTen<SP>::value;
    }
    
    if (product > INT64_MAX || product < INT64_MIN) {
        throw std::overflow_error("Multiplication result overflow");
    }

    return Price<PP>::FromRaw(static_cast<int64_t>(product));
}

template<uint PP, uint SP>
Price<PP> PxMulQty(const Price<PP>& px, const ShortQuantity<SP>& qty, RoundModel::Down) {
    int64_t price_raw = px.GetRaw();
    int64_t qty_raw = qty.GetRaw();

    __int128_t product = static_cast<__int128_t>(price_raw) * static_cast<__int128_t>(qty_raw);

    if constexpr (SP > 0) {
        product /= PowerOfTen<SP>::value;
    }
    
    if (product > INT64_MAX || product < INT64_MIN) {
        throw std::overflow_error("Multiplication result overflow");
    }

    return Price<PP>::FromRaw(static_cast<int64_t>(product));
}

template<uint PP, uint SP>
Price<PP> PxMulQty(const Price<PP>& px, const ShortQuantity<SP>& qty, RoundModel::Near) {
    int64_t price_raw = px.GetRaw();
    int64_t qty_raw = qty.GetRaw();

    __int128_t product = static_cast<__int128_t>(price_raw) * static_cast<__int128_t>(qty_raw);

    if constexpr (SP > 0) {
        static constexpr __int128_t adjust = PowerOfTen<SP>::value / 2;
        product += (product > 0) ? adjust : -adjust;
        product /= PowerOfTen<SP>::value;
    }
    
    if (product > INT64_MAX || product < INT64_MIN) {
        throw std::overflow_error("Multiplication result overflow");
    }

    return Price<PP>::FromRaw(static_cast<int64_t>(product));
}

template<uint PP, uint LP>
Price<PP> PxMulQty(const Price<PP>& px, const LongQuantity<LP>& qty, RoundModel::Up) {
    int64_t price_raw = px.GetRaw();
    LongDecimalRaw qty_raw = qty.GetRaw();
    __int128_t quantity_total = static_cast<__int128_t>(qty_raw.integer) * PowerOfTen<LP>::value + qty_raw.decimal;
    
    __int128_t product = static_cast<__int128_t>(price_raw) * quantity_total;
    
    if constexpr (LP > 0) {
        static constexpr __int128_t adjust = PowerOfTen<LP>::value - PowerOfTen<LP - 1>::value;
        product += (product > 0) ? adjust : -adjust;
        product /= PowerOfTen<LP>::value;
    }
    
    if (product > INT64_MAX || product < INT64_MIN) {
        throw std::overflow_error("Multiplication result overflow");
    }
    
    return Price<PP>::FromRaw(static_cast<int64_t>(product));
}

template<uint PP, uint LP>
Price<PP> PxMulQty(const Price<PP>& px, const LongQuantity<LP>& qty, RoundModel::Down) {
    int64_t price_raw = px.GetRaw();
    LongDecimalRaw qty_raw = qty.GetRaw();
    __int128_t quantity_total = static_cast<__int128_t>(qty_raw.integer) * PowerOfTen<LP>::value + qty_raw.decimal;
    
    __int128_t product = static_cast<__int128_t>(price_raw) * quantity_total;
    
    if constexpr (LP > 0) {
        product /= PowerOfTen<LP>::value;
    }
    
    if (product > INT64_MAX || product < INT64_MIN) {
        throw std::overflow_error("Multiplication result overflow");
    }

    return Price<PP>::FromRaw(static_cast<int64_t>(product));
}

template<uint PP, uint LP>
Price<PP> PxMulQty(const Price<PP>& px, const LongQuantity<LP>& qty, RoundModel::Near) {
    int64_t price_raw = px.GetRaw();
    LongDecimalRaw qty_raw = qty.GetRaw();
    __int128_t quantity_total = static_cast<__int128_t>(qty_raw.integer) * PowerOfTen<LP>::value + qty_raw.decimal;
    
    __int128_t product = static_cast<__int128_t>(price_raw) * quantity_total;
    
    if constexpr (LP > 0) {
        static constexpr __int128_t adjust = PowerOfTen<LP>::value / 2;
        product += (product > 0) ? adjust : -adjust;
        product /= PowerOfTen<LP>::value;
    }
    
    if (product > INT64_MAX || product < INT64_MIN) {
        throw std::overflow_error("Multiplication result overflow");
    }

    return Price<PP>::FromRaw(static_cast<int64_t>(product));
}

template<uint PP, uint SP>
ShortQuantity<SP> SQtyPxDivPx(const Price<PP>& px1, const Price<PP>& px2, RoundModel::Up) {
    int64_t px2_raw = px2.GetRaw();
    if (px2_raw == 0) {
        throw std::invalid_argument("Division by zero");
    }
    int64_t px1_raw = px1.GetRaw();
    const bool same_sign = (px1_raw > 0) == (px2_raw > 0);
    const __int128_t px1_128 = static_cast<__int128_t>(px1_raw) * PowerOfTen<SP>::value;
    const __int128_t px2_128 = static_cast<__int128_t>(px2_raw);
    const __int128_t adjusted = (10 * px1_128) + (same_sign > 0 ? (9*px2_128) : -(9*px2_128));
    __int128_t result = adjusted / (10 * px2_128);
    
    if (result > INT64_MAX || result < INT64_MIN) {
        throw std::overflow_error("Division overflow");
    }
    return ShortQuantity<SP>(static_cast<int64_t>(result));
}

template<uint PP, uint SP>
ShortQuantity<SP> SQtyPxDivPx(const Price<PP>& px1, const Price<PP>& px2, RoundModel::Down) {
    int64_t px2_raw = px2.GetRaw();
    if (px2_raw == 0) {
        throw std::invalid_argument("Division by zero");
    }
    int64_t px1_raw = px1.GetRaw();
    const __int128_t px1_128 = static_cast<__int128_t>(px1_raw) * PowerOfTen<SP>::value;
    const __int128_t px2_128 = static_cast<__int128_t>(px2_raw);
    __int128_t result = px1_128 / px2_128;
    
    if (result > INT64_MAX || result < INT64_MIN) {
        throw std::overflow_error("Division overflow");
    }
    return ShortQuantity<SP>(static_cast<int64_t>(result));
}

template<uint PP, uint SP>
ShortQuantity<SP> SQtyPxDivPx(const Price<PP>& px1, const Price<PP>& px2, RoundModel::Near) {
    int64_t px2_raw = px2.GetRaw();
    if (px2_raw == 0) {
        throw std::invalid_argument("Division by zero");
    }
    int64_t px1_raw = px1.GetRaw();
    const bool same_sign = (px1_raw > 0) == (px2_raw > 0);
    const __int128_t px1_128 = static_cast<__int128_t>(px1_raw) * PowerOfTen<SP>::value;
    const __int128_t px2_128 = static_cast<__int128_t>(px2_raw);
    const __int128_t adjusted = (10 * px1_128) + (same_sign > 0 ? (5*px2_128) : -(5*px2_128));
    __int128_t result = adjusted / (10 * px2_128);
    
    if (result > INT64_MAX || result < INT64_MIN) {
        throw std::overflow_error("Division overflow");
    }
    return ShortQuantity<SP>(static_cast<int64_t>(result));
}

template<uint PP, uint LP>
LongQuantity<LP> LQtyPxDivPx(const Price<PP>& px1, const Price<PP>& px2, RoundModel::Up) {
    int64_t px2_raw = px2.GetRaw();
    if (px2_raw == 0) {
        throw std::invalid_argument("Division by zero");
    }
    int64_t px1_raw = px1.GetRaw();
    const bool same_sign = (px1_raw > 0) == (px2_raw > 0);
    const __int128_t px1_128 = static_cast<__int128_t>(px1_raw) * PowerOfTen<LP>::value;
    const __int128_t px2_128 = static_cast<__int128_t>(px2_raw);
    const __int128_t adjusted = (10 * px1_128) + (same_sign > 0 ? (9*px2_128) : -(9*px2_128));
    __int128_t result = adjusted / (10 * px2_128);

    __int128_t integer = result / PowerOfTen<LP>::value;
    __int128_t decimal = result % PowerOfTen<LP>::value;
    return LongQuantity<LP>({static_cast<int64_t>(integer), static_cast<int64_t>(decimal)});
}

template<uint PP, uint LP>
LongQuantity<LP> LQtyPxDivPx(const Price<PP>& px1, const Price<PP>& px2, RoundModel::Down) {
    int64_t px2_raw = px2.GetRaw();
    if (px2_raw == 0) {
        throw std::invalid_argument("Division by zero");
    }
    int64_t px1_raw = px1.GetRaw();
    const __int128_t px1_128 = static_cast<__int128_t>(px1_raw) * PowerOfTen<LP>::value;
    const __int128_t px2_128 = static_cast<__int128_t>(px2_raw);
    __int128_t result = px1_128 / px2_128;
    
    __int128_t integer = result / PowerOfTen<LP>::value;
    __int128_t decimal = result % PowerOfTen<LP>::value;
    return LongQuantity<LP>({static_cast<int64_t>(integer), static_cast<int64_t>(decimal)});
}

template<uint PP, uint LP>
LongQuantity<LP> LQtyPxDivPx(const Price<PP>& px1, const Price<PP>& px2, RoundModel::Near) {
    int64_t px2_raw = px2.GetRaw();
    if (px2_raw == 0) {
        throw std::invalid_argument("Division by zero");
    }
    int64_t px1_raw = px1.GetRaw();
    const bool same_sign = (px1_raw > 0) == (px2_raw > 0);
    const __int128_t px1_128 = static_cast<__int128_t>(px1_raw) * PowerOfTen<LP>::value;
    const __int128_t px2_128 = static_cast<__int128_t>(px2_raw);
    const __int128_t adjusted = (10 * px1_128) + (same_sign > 0 ? (5*px2_128) : -(5*px2_128));
    __int128_t result = adjusted / (10 * px2_128);
    
    __int128_t integer = result / PowerOfTen<LP>::value;
    __int128_t decimal = result % PowerOfTen<LP>::value;
    return LongQuantity<LP>({static_cast<int64_t>(integer), static_cast<int64_t>(decimal)});
}

template<uint PP>
int64_t IntPxDivPx(const Price<PP>& px1, const Price<PP>& px2, RoundModel::Up) {
    int64_t px2_raw = px2.GetRaw();
    if (px2_raw == 0) {
        throw std::invalid_argument("Division by zero");
    }
    int64_t px1_raw = px1.GetRaw();
    const bool same_sign = (px1_raw > 0) == (px2_raw > 0);
    const __int128_t px1_128 = static_cast<__int128_t>(px1_raw);
    const __int128_t px2_128 = static_cast<__int128_t>(px2_raw);
    const __int128_t adjusted = (10 * px1_128) + (same_sign > 0 ? (9*px2_128) : -(9*px2_128));
    __int128_t result = adjusted / (10 * px2_128);
    
    if (result > INT64_MAX || result < INT64_MIN) {
        throw std::overflow_error("Division overflow");
    }
    return static_cast<int64_t>(result);
}

template<uint PP>
int64_t IntPxDivPx(const Price<PP>& px1, const Price<PP>& px2, RoundModel::Down) {
    int64_t px2_raw = px2.GetRaw();
    if (px2_raw == 0) {
        throw std::invalid_argument("Division by zero");
    }
    int64_t px1_raw = px1.GetRaw();
    const __int128_t px1_128 = static_cast<__int128_t>(px1_raw);
    const __int128_t px2_128 = static_cast<__int128_t>(px2_raw);
    __int128_t result = px1_128 / px2_128;

    if (result > INT64_MAX || result < INT64_MIN) {
        throw std::overflow_error("Division overflow");
    }
    return static_cast<int64_t>(result);
}

template<uint PP>
int64_t IntPxDivPx(const Price<PP>& px1, const Price<PP>& px2, RoundModel::Near) {
    int64_t px2_raw = px2.GetRaw();
    if (px2_raw == 0) {
        throw std::invalid_argument("Division by zero");
    }
    int64_t px1_raw = px1.GetRaw();
    const bool same_sign = (px1_raw > 0) == (px2_raw > 0);
    const __int128_t px1_128 = static_cast<__int128_t>(px1_raw);
    const __int128_t px2_128 = static_cast<__int128_t>(px2_raw);
    const __int128_t adjusted = (10 * px1_128) + (same_sign > 0 ? (5*px2_128) : -(5*px2_128));
    __int128_t result = adjusted / (10 * px2_128);
    
    if (result > INT64_MAX || result < INT64_MIN) {
        throw std::overflow_error("Division overflow");
    }
    return static_cast<int64_t>(result);
}

#endif
