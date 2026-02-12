#pragma once

#include <stdint.h>
#include <type_traits>

template<int INT_BITS, int FRAC_BITS>
class QNumber {
private:
    static constexpr int total_bits = INT_BITS + FRAC_BITS;
    static constexpr int total_bytes = (total_bits + 7) / 8;
    
    using StorageType = typename std::conditional<
        total_bytes == 1, int8_t,
        typename std::conditional<
            total_bytes == 2, int16_t,
            typename std::conditional<
                total_bytes <= 4, int32_t,
                int64_t
            >::type
        >::type
    >::type;
    
    StorageType value;
    
    static constexpr double Scale = (double)(1LL << FRAC_BITS);
    
    StorageType sign_extend(StorageType val) const {
        if (total_bits < sizeof(StorageType) * 8) {
            StorageType signBit = (StorageType)1 << (total_bits - 1);
            if (val & signBit) {
                StorageType mask = ~((1LL << total_bits) - 1);
                val |= mask;
            }
        }
        return val;
    }
    
    StorageType get_mask() const {
        if (total_bits < sizeof(StorageType) * 8) {
            return ((StorageType)1 << total_bits) - 1;
        }
        return ~(StorageType)0;
    }

public:
    QNumber() : value(0) {}
    
    static QNumber from_raw(StorageType raw) {
        QNumber result;
        result.value = raw;
        return result;
    }
    
    QNumber(int8_t v) : value((StorageType)v << FRAC_BITS) { value &= get_mask(); }
    QNumber(int16_t v) : value((StorageType)v << FRAC_BITS) { value &= get_mask(); }
    QNumber(int32_t v) : value((StorageType)v << FRAC_BITS) { value &= get_mask(); }
    QNumber(int64_t v) : value((StorageType)v << FRAC_BITS) { value &= get_mask(); }
    
    QNumber(uint8_t v) : value((StorageType)v << FRAC_BITS) { value &= get_mask(); }
    QNumber(uint16_t v) : value((StorageType)v << FRAC_BITS) { value &= get_mask(); }
    QNumber(uint32_t v) : value((StorageType)v << FRAC_BITS) { value &= get_mask(); }
    
    QNumber(float v) {
        double scaled = v * Scale;
        value = (StorageType)(scaled + (scaled >= 0 ? 0.5 : -0.5));
        value &= get_mask();
    }
    
    QNumber(double v) {
        double scaled = v * Scale;
        value = (StorageType)(scaled + (scaled >= 0 ? 0.5 : -0.5));
        value &= get_mask();
    }
    
    explicit operator int8_t() const {
        StorageType extended = sign_extend(value);
        StorageType rounded = extended + ((StorageType)1 << (FRAC_BITS - 1));
        return (int8_t)(rounded >> FRAC_BITS);
    }
    
    explicit operator int16_t() const {
        StorageType extended = sign_extend(value);
        StorageType rounded = extended + ((StorageType)1 << (FRAC_BITS - 1));
        return (int16_t)(rounded >> FRAC_BITS);
    }
    
    explicit operator int32_t() const {
        StorageType extended = sign_extend(value);
        StorageType rounded = extended + ((StorageType)1 << (FRAC_BITS - 1));
        return (int32_t)(rounded >> FRAC_BITS);
    }
    
    explicit operator int64_t() const {
        StorageType extended = sign_extend(value);
        StorageType rounded = extended + ((StorageType)1 << (FRAC_BITS - 1));
        return (int64_t)(rounded >> FRAC_BITS);
    }
    
    explicit operator uint8_t() const {
        StorageType rounded = value + ((StorageType)1 << (FRAC_BITS - 1));
        return (uint8_t)(rounded >> FRAC_BITS);
    }
    
    explicit operator uint16_t() const {
        StorageType rounded = value + ((StorageType)1 << (FRAC_BITS - 1));
        return (uint16_t)(rounded >> FRAC_BITS);
    }
    
    explicit operator uint32_t() const {
        StorageType rounded = value + ((StorageType)1 << (FRAC_BITS - 1));
        return (uint32_t)(rounded >> FRAC_BITS);
    }
    
    explicit operator float() const {
        StorageType extended = sign_extend(value);
        return (float)extended / (float)Scale;
    }
    
    explicit operator double() const {
        StorageType extended = sign_extend(value);
        return (double)extended / Scale;
    }
    
    QNumber operator+(const QNumber& other) const {
        QNumber result;
        result.value = (value + other.value) & get_mask();
        return result;
    }
    
    QNumber operator-(const QNumber& other) const {
        QNumber result;
        result.value = (value - other.value) & get_mask();
        return result;
    }
    
    QNumber operator*(const QNumber& other) const {
        QNumber result;
        int64_t temp = ((int64_t)sign_extend(value) * (int64_t)sign_extend(other.value));
        result.value = (StorageType)(temp >> FRAC_BITS) & get_mask();
        return result;
    }
    
    QNumber operator/(const QNumber& other) const {
        QNumber result;
        int64_t temp = ((int64_t)sign_extend(value) << FRAC_BITS);
        result.value = (StorageType)(temp / sign_extend(other.value)) & get_mask();
        return result;
    }
    
    QNumber operator-() const {
        QNumber result;
        result.value = (-value) & get_mask();
        return result;
    }
    
    QNumber& operator+=(const QNumber& other) {
        value = (value + other.value) & get_mask();
        return *this;
    }
    
    QNumber& operator-=(const QNumber& other) {
        value = (value - other.value) & get_mask();
        return *this;
    }
    
    QNumber& operator*=(const QNumber& other) {
        *this = *this * other;
        return *this;
    }
    
    QNumber& operator/=(const QNumber& other) {
        *this = *this / other;
        return *this;
    }
    
    bool operator==(const QNumber& other) const { return value == other.value; }
    bool operator!=(const QNumber& other) const { return value != other.value; }
    bool operator<(const QNumber& other) const { 
        return sign_extend(value) < sign_extend(other.value); 
    }
    bool operator<=(const QNumber& other) const { 
        return sign_extend(value) <= sign_extend(other.value); 
    }
    bool operator>(const QNumber& other) const { 
        return sign_extend(value) > sign_extend(other.value); 
    }
    bool operator>=(const QNumber& other) const { 
        return sign_extend(value) >= sign_extend(other.value); 
    }
    
    StorageType raw() const { return value; }
    static constexpr int bytes() { return total_bytes; }
    static constexpr int bits() { return total_bits; }
    static constexpr int integer_bits() { return INT_BITS; }
    static constexpr int fractional_bits() { return FRAC_BITS; }
    
    static QNumber min() {
        QNumber result;
        result.value = (StorageType)1 << (total_bits - 1);
        return result;
    }
    
    static QNumber max() {
        QNumber result;
        result.value = ((StorageType)1 << (total_bits - 1)) - 1;
        return result;
    }
    
    void toBytes(uint8_t* buffer) const {
        for (int i = 0; i < total_bytes; i++) {
            buffer[i] = (value >> (i * 8)) & 0xFF;
        }
    }    

    static constexpr int byte_size() { return total_bytes; }
    
    static QNumber from_bytes(const uint8_t* buffer) {
        QNumber result;
        result.value = 0;
        for (int i = 0; i < total_bytes; i++) {
            result.value |= ((StorageType)buffer[i]) << (i * 8);
        }
        result.value = result.sign_extend(result.value);
        return result;
    }
};

using QNumber3_4   = QNumber<3, 4>;
using QNumber4_4   = QNumber<4, 4>;
using QNumber7_1   = QNumber<7, 1>;
using QNumber1_7   = QNumber<1, 7>;
using QNumber6_2   = QNumber<6, 2>;
using QNumber2_6   = QNumber<2, 6>;
using QNumber7_8   = QNumber<7, 8>;
using QNumber8_8   = QNumber<8, 8>;
using QNumber9_6   = QNumber<9, 6>;
using QNumber10_6  = QNumber<10, 6>;
using QNumber15_1  = QNumber<15, 1>;
using QNumber1_15  = QNumber<1, 15>;
using QNumber12_12 = QNumber<12, 12>;
using QNumber16_8  = QNumber<16, 8>;
using QNumber8_16  = QNumber<8, 16>;
using QNumber15_16 = QNumber<15, 16>;
using QNumber16_16 = QNumber<16, 16>;
using QNumber24_8  = QNumber<24, 8>;
using QNumber8_24  = QNumber<8, 24>;
using QNumber31_32 = QNumber<31, 32>;
using QNumber32_32 = QNumber<32, 32>;

