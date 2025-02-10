#ifndef __DECIMAL_CNL__
#define __DECIMAL_CNL__
#include <cnl/scaled_integer.h>

class DecimalCNL {
private:
    typedef typename cnl::scaled_integer<int64_t, cnl::power<-8, 10>> data_type;
    data_type data_;
public:
    DecimalCNL(const double s) : data_(s) {}
    DecimalCNL(const data_type& data) : data_(data) {}
    DecimalCNL(data_type&& data) : data_(std::move(data)) {}
    DecimalCNL(const DecimalCNL& other) : data_(other.data_) {}
    DecimalCNL(DecimalCNL&& other) noexcept : data_(std::move(other.data_)) {}


    DecimalCNL operator+(const DecimalCNL& other) const {
        return DecimalCNL(data_ + other.data_);
    }
    DecimalCNL operator-(const DecimalCNL& other) const {
        return DecimalCNL(data_ - other.data_);
    }
    DecimalCNL operator*(const DecimalCNL& other) const {
        data_type r = data_ * other.data_;
        return DecimalCNL(std::move(r));
    }
    DecimalCNL operator/(const DecimalCNL& other) const {
        data_type r = data_ / other.data_;
        return DecimalCNL(std::move(r));
    }   


    DecimalCNL& operator+=(const DecimalCNL& other) {
        data_ += other.data_;
        return *this;
    }
    DecimalCNL& operator-=(const DecimalCNL& other) {
        data_ -= other.data_;
        return *this;
    }
    DecimalCNL& operator*=(const DecimalCNL& other) {
        data_ *= other.data_;
        return *this;
    }
    DecimalCNL& operator/=(const DecimalCNL& other) {
        data_ /= other.data_;
        return *this;
    }


    bool operator==(const DecimalCNL& other) const {
        return data_ == other.data_;
    }
    bool operator!=(const DecimalCNL& other) const {
        return data_ != other.data_;
    }
    bool operator<(const DecimalCNL& other) const {
        return data_ < other.data_;
    }
    bool operator<=(const DecimalCNL& other) const {
        return data_ <= other.data_;
    }
    bool operator>(const DecimalCNL& other) const {
        return data_ > other.data_;
    }
    bool operator>=(const DecimalCNL& other) const {
        return data_ >= other.data_;
    }
};

#endif