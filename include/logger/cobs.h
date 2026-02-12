#pragma once

#include <stdint.h>
#include <stddef.h>

class COBS {
public:
    static size_t encode(const uint8_t* source, size_t source_length, uint8_t* dest) {
        if (source_length == 0) {
            dest[0] = 0x01;
            return 1;
        }
        
        uint8_t* dest_start = dest;
        uint8_t* code_ptr = dest++;
        uint8_t code = 0x01;
        
        for (size_t i = 0; i < source_length; i++) {
            if (source[i] == 0x00) {
                *code_ptr = code;
                code_ptr = dest++;
                code = 0x01;
            } else {
                *dest++ = source[i];
                code++;
                
                if (code == 0xFF) {
                    *code_ptr = code;
                    code_ptr = dest++;
                    code = 0x01;
                }
            }
        }
        
        *code_ptr = code;
        
        return dest - dest_start;
    }
    
    static size_t decode(const uint8_t* source, size_t source_length, uint8_t* dest) {
        if (source_length == 0) {
            return 0;
        }
        
        uint8_t* dest_start = dest;
        const uint8_t* source_end = source + source_length;
        
        while (source < source_end) {
            uint8_t code = *source++;
            
            if (code == 0x00 || source + code - 1 > source_end) {
                return 0;
            }
            
            for (uint8_t i = 1; i < code; i++) {
                *dest++ = *source++;
            }
            
            if (code < 0xFF && source < source_end) {
                *dest++ = 0x00;
            }
        }
        
        return dest - dest_start;
    }
    
    static size_t max_encoded_size(size_t source_length) {
        return source_length + (source_length / 254) + 1;
    }
};

