#pragma once

#include <stdint.h>
#include <stddef.h>

// CRC-16-CCITT
// polynomial 0x1021 (x^16 + x^12 + x^5 + 1)
// initial value 0xFFFF

class CRC16 {
private:
    static constexpr uint16_t POLYNOMIAL = 0x1021;
    static constexpr uint16_t INITIAL_VALUE = 0xFFFF;
    
public:
    static uint16_t calculate(const uint8_t* data, size_t length) {
        uint16_t crc = INITIAL_VALUE;
        
        for (size_t i = 0; i < length; i++) {
            crc ^= (uint16_t)data[i] << 8;
            
            for (int j = 0; j < 8; j++) {
                if (crc & 0x8000) {
                    crc = (crc << 1) ^ POLYNOMIAL;
                } else {
                    crc = crc << 1;
                }
            }
        }
        
        return crc;
    }
    
    static bool verify(const uint8_t* data, size_t length) {
        if (length < 2) return false;
        
        uint16_t calculated = calculate(data, length - 2);
        
        uint16_t stored = ((uint16_t)data[length - 2] << 8) | data[length - 1];
        
        return calculated == stored;
    }
    
    static void append(uint8_t* buffer, size_t length) {
        uint16_t crc = calculate(buffer, length);
        buffer[length] = (crc >> 8) & 0xFF;
        buffer[length + 1] = crc & 0xFF;
    }
};

