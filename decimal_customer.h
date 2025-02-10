#ifndef __DECIMAL_CUSTOMER__
#define __DECIMAL_CUSTOMER__

#include <cmath>

class DecimalCustomer {
private:
    int64_t data_;
    static constexpr int64_t SCALING_FACTOR = 100000000LL;

    explicit DecimalCustomer(int64_t rawValue) : data_(rawValue) {}

public:
    DecimalCustomer() : data_(0) {}

    static DecimalCustomer fromRaw(int64_t rawValue) { return DecimalCustomer(rawValue); }
    static DecimalCustomer fromInteger(int64_t integer) { return DecimalCustomer(integer * SCALING_FACTOR); }
    static DecimalCustomer fromDouble(double d) {
        return DecimalCustomer(static_cast<int64_t>(std::round(d * SCALING_FACTOR)));
    }

    double toDouble() const { return static_cast<double>(data_) / SCALING_FACTOR; }
    int64_t getRawValue() const { return data_; }


    DecimalCustomer operator+(const DecimalCustomer& rhs) const {
        return DecimalCustomer(data_ + rhs.data_);
    }
    DecimalCustomer operator-(const DecimalCustomer& rhs) const {
        return DecimalCustomer(data_ - rhs.data_);
    }
    DecimalCustomer operator*(const DecimalCustomer& rhs) const {
        __int128_t result = static_cast<__int128_t>(data_) * rhs.data_;
        return DecimalCustomer(static_cast<int64_t>(result / SCALING_FACTOR));
    }
    DecimalCustomer operator/(const DecimalCustomer& rhs) const {
        __int128_t dividend = static_cast<__int128_t>(data_) * SCALING_FACTOR;
        return DecimalCustomer(static_cast<int64_t>(dividend / rhs.data_));
    }


    DecimalCustomer& operator+=(const DecimalCustomer& rhs) {
        data_ += rhs.data_;
        return *this;
    }
    DecimalCustomer& operator-=(const DecimalCustomer& rhs) {
        data_ -= rhs.data_;
        return *this;
    }
    DecimalCustomer& operator*=(const DecimalCustomer& rhs) {
        __int128_t result = static_cast<__int128_t>(data_) * rhs.data_;
        data_ = static_cast<int64_t>(result / SCALING_FACTOR);
        return *this;
    }
    DecimalCustomer& operator/=(const DecimalCustomer& rhs) {
        __int128_t dividend = static_cast<__int128_t>(data_) * SCALING_FACTOR;
        data_ = static_cast<int64_t>(dividend / rhs.data_);
        return *this;
    }


    bool operator==(const DecimalCustomer& rhs) const { return data_ == rhs.data_; }
    bool operator!=(const DecimalCustomer& rhs) const { return data_ != rhs.data_; }
    bool operator<(const DecimalCustomer& rhs) const { return data_ < rhs.data_; }
    bool operator<=(const DecimalCustomer& rhs) const { return data_ <= rhs.data_; }
    bool operator>(const DecimalCustomer& rhs) const { return data_ > rhs.data_; }
    bool operator>=(const DecimalCustomer& rhs) const { return data_ >= rhs.data_; }
};

#endif // DECIMAL_H