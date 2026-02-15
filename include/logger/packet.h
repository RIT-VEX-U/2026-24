#pragma once

#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include "crc16.h"
#include "cobs.h"
#include "fixedpoint.h"

// doesn't include crc and id
#define MAX_DATA_BYTES 192

enum class SerialLoggerTypeCode {
    U8, U16, U32, U64,
    I8, I16, I32, I64,
    F32, F64,
    Q3_4, Q4_4, Q7_1, Q1_7, Q6_2, Q2_6,
    Q7_8, Q8_8, Q15_1, Q1_15, Q10_6, Q9_6,
    Q12_12, Q16_8, Q8_16,
    Q15_16, Q16_16, Q24_8, Q8_24,
    Q31_32, Q32_32,
    UNKNOWN
};

struct SerialLoggerField {
    std::string name;
    SerialLoggerTypeCode type;
    std::string group;
    
    size_t get_size() const;
    const char* get_type_name() const;
};


struct SerialLoggerSchema {
    uint8_t message_id;
    std::vector<SerialLoggerField> fields;
    
    size_t get_total_size() const {
        size_t total = 0;
        for (const auto& field : fields) {
            total += field.get_size();
        }
        return total;
    }
};

class SerialLoggerEncoder {
private:
    std::map<uint8_t, SerialLoggerSchema> schemas;
    
    SerialLoggerTypeCode parse_type(const char* type_str) const;
    size_t serialize_value(uint8_t* buffer, const SerialLoggerField& field, double value) const;
    
public:
    bool register_schema(uint8_t message_id, const char* schema_str);
    
    const SerialLoggerSchema* get_schema(uint8_t message_id) const;
    
    bool has_schema(uint8_t message_id) const {
        return schemas.find(message_id) != schemas.end();
    }
    
    size_t encode_schema_packet(uint8_t message_id, const char* schema_str, uint8_t* output);
    
    size_t encode_data_packet(uint8_t message_id, const double* values, size_t num_values, uint8_t* output);
    
    
    class SerialLoggerDataBuilder {
    private:
        SerialLoggerEncoder* encoder;
        uint8_t raw_buffer[MAX_DATA_BYTES + 3];
        size_t offset;
        size_t field_index;
        const SerialLoggerSchema* schema;
        
    public:
        SerialLoggerDataBuilder(SerialLoggerEncoder* enc, uint8_t message_id);
        
        SerialLoggerDataBuilder& add(uint8_t value);
        SerialLoggerDataBuilder& add(uint16_t value);
        SerialLoggerDataBuilder& add(uint32_t value);
        SerialLoggerDataBuilder& add(uint64_t value);
        SerialLoggerDataBuilder& add(int8_t value);
        SerialLoggerDataBuilder& add(int16_t value);
        SerialLoggerDataBuilder& add(int32_t value);
        SerialLoggerDataBuilder& add(int64_t value);
        SerialLoggerDataBuilder& add(float value);
        SerialLoggerDataBuilder& add(double value);

        template<int INT_BITS, int FRAC_BITS>
        SerialLoggerDataBuilder& add(const QNumber<INT_BITS, FRAC_BITS>& value) {
            if (!schema || field_index >= schema->fields.size()) return *this;
            value.to_bytes(raw_buffer + offset);
            offset += value.byte_size();
            field_index++;
            return *this;
        }
        size_t send(uint8_t* output);
        
        bool is_valid() const { return schema != nullptr; }
    };
    
    SerialLoggerDataBuilder build(uint8_t message_id) {
        return SerialLoggerDataBuilder(this, message_id);
    }
};

