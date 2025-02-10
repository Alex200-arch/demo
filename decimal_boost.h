#ifndef __DECIMAL_BOOST__
#define __DECIMAL_BOOST__
#include <boost/multiprecision/cpp_dec_float.hpp>

class DecimalBoost {
private:
    typedef boost::multiprecision::cpp_dec_float_50 data_type;
    data_type data_;
public:
    DecimalBoost(const char* s) : data_(s) {}
    DecimalBoost(const data_type& data) : data_(data) {}
    DecimalBoost(data_type&& data) : data_(std::move(data)) {}
    DecimalBoost(const DecimalBoost& other) : data_(other.data_) {}
    DecimalBoost(DecimalBoost&& other) noexcept : data_(std::move(other.data_)) {}


    DecimalBoost operator+(const DecimalBoost& other) const {
        return DecimalBoost(data_ + other.data_);
    }
    DecimalBoost operator-(const DecimalBoost& other) const {
        return DecimalBoost(data_ - other.data_);
    }
    DecimalBoost operator*(const DecimalBoost& other) const {
        return DecimalBoost(data_ * other.data_);
    }
    DecimalBoost operator/(const DecimalBoost& other) const {
        return DecimalBoost(data_ / other.data_);
    }


    DecimalBoost& operator+=(const DecimalBoost& other) {
        data_ += other.data_;
        return *this;
    }
    DecimalBoost& operator-=(const DecimalBoost& other) {
        data_ -= other.data_;
        return *this;
    }
    DecimalBoost& operator*=(const DecimalBoost& other) {
        data_ *= other.data_;
        return *this;
    }
    DecimalBoost& operator/=(const DecimalBoost& other) {
        data_ /= other.data_;
        return *this;
    }


    bool operator==(const DecimalBoost& other) const {
        return data_ == other.data_;
    }
    bool operator!=(const DecimalBoost& other) const {
        return data_ != other.data_;
    }
    bool operator<(const DecimalBoost& other) const {
        return data_ < other.data_;
    }
    bool operator<=(const DecimalBoost& other) const {
        return data_ <= other.data_;
    }
    bool operator>(const DecimalBoost& other) const {
        return data_ > other.data_;
    }
    bool operator>=(const DecimalBoost& other) const {
        return data_ >= other.data_;
    }
};

#endif