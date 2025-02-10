#ifndef __DECIMAL_FIX16__
#define __DECIMAL_FIX16__
#include <libfixmath/fix16.h>

class DecimalFix16 {
private:
    typedef fix16_t data_type;
    data_type data_;
public:
    DecimalFix16(const double s) : data_(fix16_from_dbl(s)) {}
    DecimalFix16(const data_type& data) : data_(data) {}
    DecimalFix16(data_type&& data) : data_(std::move(data)) {}
    DecimalFix16(const DecimalFix16& other) : data_(other.data_) {}
    DecimalFix16(DecimalFix16&& other) noexcept : data_(std::move(other.data_)) {}


    DecimalFix16 operator+(const DecimalFix16& other) const {
        data_type r = fix16_add(data_, other.data_);
        return DecimalFix16(std::move(r));
    }
    DecimalFix16 operator-(const DecimalFix16& other) const {
        data_type r = fix16_sub(data_, other.data_);
        return DecimalFix16(std::move(r));
    }
    DecimalFix16 operator*(const DecimalFix16& other) const {
        data_type r = fix16_mul(data_, other.data_);
        return DecimalFix16(std::move(r));
    }
    DecimalFix16 operator/(const DecimalFix16& other) const {
        data_type r = fix16_div(data_, other.data_);
        return DecimalFix16(std::move(r));
    }   


    DecimalFix16& operator+=(const DecimalFix16& other) {
        data_ = fix16_add(data_, other.data_);
        return *this;
    }
    DecimalFix16& operator-=(const DecimalFix16& other) {
        data_ = fix16_sub(data_, other.data_);
        return *this;
    }
    DecimalFix16& operator*=(const DecimalFix16& other) {
        data_ = fix16_mul(data_, other.data_);
        return *this;
    }
    DecimalFix16& operator/=(const DecimalFix16& other) {
        data_ = fix16_div(data_, other.data_);
        return *this;
    }


    bool operator==(const DecimalFix16& other) const {
        return data_ == other.data_;
    }
    bool operator!=(const DecimalFix16& other) const {
        return data_ != other.data_;
    }
    bool operator<(const DecimalFix16& other) const {
        return data_ < other.data_;
    }
    bool operator<=(const DecimalFix16& other) const {
        return data_ <= other.data_;
    }
    bool operator>(const DecimalFix16& other) const {
        return data_ > other.data_;
    }
    bool operator>=(const DecimalFix16& other) const {
        return data_ >= other.data_;
    }
};

#endif