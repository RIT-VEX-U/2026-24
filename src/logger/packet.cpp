#include "logger/packet.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


size_t SerialLoggerField::get_size() const {
    switch (type) {
        case SerialLoggerTypeCode::U8:
        case SerialLoggerTypeCode::I8:
        case SerialLoggerTypeCode::Q3_4:
        case SerialLoggerTypeCode::Q4_4:
        case SerialLoggerTypeCode::Q7_1:
        case SerialLoggerTypeCode::Q1_7:
        case SerialLoggerTypeCode::Q6_2:
        case SerialLoggerTypeCode::Q2_6:
            return 1;
            
        case SerialLoggerTypeCode::U16:
        case SerialLoggerTypeCode::I16:
        case SerialLoggerTypeCode::Q7_8:
        case SerialLoggerTypeCode::Q8_8:
        case SerialLoggerTypeCode::Q15_1:
        case SerialLoggerTypeCode::Q1_15:
        case SerialLoggerTypeCode::Q9_6:
        case SerialLoggerTypeCode::Q10_6:
            return 2;
            
        case SerialLoggerTypeCode::Q12_12:
        case SerialLoggerTypeCode::Q16_8:
        case SerialLoggerTypeCode::Q8_16:
            return 3;
            
        case SerialLoggerTypeCode::U32:
        case SerialLoggerTypeCode::I32:
        case SerialLoggerTypeCode::F32:
        case SerialLoggerTypeCode::Q15_16:
        case SerialLoggerTypeCode::Q16_16:
        case SerialLoggerTypeCode::Q24_8:
        case SerialLoggerTypeCode::Q8_24:
            return 4;
            
        case SerialLoggerTypeCode::U64:
        case SerialLoggerTypeCode::I64:
        case SerialLoggerTypeCode::F64:
        case SerialLoggerTypeCode::Q31_32:
        case SerialLoggerTypeCode::Q32_32:
            return 8;
            
        default:
            return 0;
    }
}

const char* SerialLoggerField::get_type_name() const {
    switch (type) {
        case SerialLoggerTypeCode::U8: return "u8";
        case SerialLoggerTypeCode::U16: return "u16";
        case SerialLoggerTypeCode::U32: return "u32";
        case SerialLoggerTypeCode::U64: return "u64";
        case SerialLoggerTypeCode::I8: return "i8";
        case SerialLoggerTypeCode::I16: return "i16";
        case SerialLoggerTypeCode::I32: return "i32";
        case SerialLoggerTypeCode::I64: return "i64";
        case SerialLoggerTypeCode::F32: return "f32";
        case SerialLoggerTypeCode::F64: return "f64";
        case SerialLoggerTypeCode::Q3_4: return "Q3_4";
        case SerialLoggerTypeCode::Q4_4: return "Q4_4";
        case SerialLoggerTypeCode::Q7_1: return "Q7_1";
        case SerialLoggerTypeCode::Q1_7: return "Q1_7";
        case SerialLoggerTypeCode::Q6_2: return "Q6_2";
        case SerialLoggerTypeCode::Q2_6: return "Q2_6";
        case SerialLoggerTypeCode::Q7_8: return "Q7_8";
        case SerialLoggerTypeCode::Q8_8: return "Q8_8";
        case SerialLoggerTypeCode::Q15_1: return "Q15_1";
        case SerialLoggerTypeCode::Q1_15: return "Q1_15";
        case SerialLoggerTypeCode::Q9_6: return "Q9_6";
        case SerialLoggerTypeCode::Q10_6: return "Q10_6";
        case SerialLoggerTypeCode::Q12_12: return "Q12_12";
        case SerialLoggerTypeCode::Q16_8: return "Q16_8";
        case SerialLoggerTypeCode::Q8_16: return "Q8_16";
        case SerialLoggerTypeCode::Q15_16: return "Q15_16";
        case SerialLoggerTypeCode::Q16_16: return "Q16_16";
        case SerialLoggerTypeCode::Q24_8: return "Q24_8";
        case SerialLoggerTypeCode::Q8_24: return "Q8_24";
        case SerialLoggerTypeCode::Q31_32: return "Q31_32";
        case SerialLoggerTypeCode::Q32_32: return "Q32_32";
        default: return "unknown";
    }
}


SerialLoggerTypeCode SerialLoggerEncoder::parse_type(const char* type_str) const {
    if (strcmp(type_str, "u8") == 0) return SerialLoggerTypeCode::U8;
    if (strcmp(type_str, "u16") == 0) return SerialLoggerTypeCode::U16;
    if (strcmp(type_str, "u32") == 0) return SerialLoggerTypeCode::U32;
    if (strcmp(type_str, "u64") == 0) return SerialLoggerTypeCode::U64;
    if (strcmp(type_str, "i8") == 0) return SerialLoggerTypeCode::I8;
    if (strcmp(type_str, "i16") == 0) return SerialLoggerTypeCode::I16;
    if (strcmp(type_str, "i32") == 0) return SerialLoggerTypeCode::I32;
    if (strcmp(type_str, "i64") == 0) return SerialLoggerTypeCode::I64;
    if (strcmp(type_str, "f32") == 0) return SerialLoggerTypeCode::F32;
    if (strcmp(type_str, "f64") == 0) return SerialLoggerTypeCode::F64;
    if (strcmp(type_str, "Q3_4") == 0) return SerialLoggerTypeCode::Q3_4;
    if (strcmp(type_str, "Q4_4") == 0) return SerialLoggerTypeCode::Q4_4;
    if (strcmp(type_str, "Q7_1") == 0) return SerialLoggerTypeCode::Q7_1;
    if (strcmp(type_str, "Q1_7") == 0) return SerialLoggerTypeCode::Q1_7;
    if (strcmp(type_str, "Q6_2") == 0) return SerialLoggerTypeCode::Q6_2;
    if (strcmp(type_str, "Q2_6") == 0) return SerialLoggerTypeCode::Q2_6;
    if (strcmp(type_str, "Q7_8") == 0) return SerialLoggerTypeCode::Q7_8;
    if (strcmp(type_str, "Q8_8") == 0) return SerialLoggerTypeCode::Q8_8;
    if (strcmp(type_str, "Q15_1") == 0) return SerialLoggerTypeCode::Q15_1;
    if (strcmp(type_str, "Q1_15") == 0) return SerialLoggerTypeCode::Q1_15;
    if (strcmp(type_str, "Q9_6") == 0) return SerialLoggerTypeCode::Q9_6;
    if (strcmp(type_str, "Q10_6") == 0) return SerialLoggerTypeCode::Q10_6;
    if (strcmp(type_str, "Q12_12") == 0) return SerialLoggerTypeCode::Q12_12;
    if (strcmp(type_str, "Q16_8") == 0) return SerialLoggerTypeCode::Q16_8;
    if (strcmp(type_str, "Q8_16") == 0) return SerialLoggerTypeCode::Q8_16;
    if (strcmp(type_str, "Q15_16") == 0) return SerialLoggerTypeCode::Q15_16;
    if (strcmp(type_str, "Q16_16") == 0) return SerialLoggerTypeCode::Q16_16;
    if (strcmp(type_str, "Q24_8") == 0) return SerialLoggerTypeCode::Q24_8;
    if (strcmp(type_str, "Q8_24") == 0) return SerialLoggerTypeCode::Q8_24;
    if (strcmp(type_str, "Q31_32") == 0) return SerialLoggerTypeCode::Q31_32;
    if (strcmp(type_str, "Q32_32") == 0) return SerialLoggerTypeCode::Q32_32;
    return SerialLoggerTypeCode::UNKNOWN;
}

bool SerialLoggerEncoder::register_schema(uint8_t message_id, const char* schema_str) {
    SerialLoggerSchema schema;
    schema.message_id = message_id | 0x80;
    
    char buffer[256];
    strncpy(buffer, schema_str, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char* token = strtok(buffer, ",");
    while (token != NULL) {
        while (*token && isspace(*token)) token++;
        
        if (*token == '\0') {
            token = strtok(NULL, ",");
            continue;
        }
        
        char* colon_pos = strchr(token, ':');
        if (colon_pos == NULL) {
            printf("Invalid field format: %s\n", token);
            return false;
        }
        
        *colon_pos = '\0';
        char* name_str = token;
        char* type_str = colon_pos + 1;
        
        char* end = name_str + strlen(name_str) - 1;
        while (end > name_str && isspace(*end)) *end-- = '\0';
        
        while (*type_str && isspace(*type_str)) type_str++;
        end = type_str + strlen(type_str) - 1;
        while (end > type_str && isspace(*end)) *end-- = '\0';
        
        SerialLoggerField field;
        
        char* dot_pos = strchr(name_str, '.');
        if (dot_pos != NULL) {
            *dot_pos = '\0';
            field.group = name_str;
            field.name = dot_pos + 1;
        } else {
            field.group = "";
            field.name = name_str;
        }
        
        field.type = parse_type(type_str);
        if (field.type == SerialLoggerTypeCode::UNKNOWN) {
            printf("Unknown type: %s\n", type_str);
            return false;
        }
        
        schema.fields.push_back(field);
        
        token = strtok(NULL, ",");
    }
    
    if (schema.get_total_size() > MAX_DATA_BYTES) {
        printf("Schema too large: %zu bytes (max %d)\n", schema.get_total_size(), MAX_DATA_BYTES);
        return false;
    }
    
    schemas[message_id & 0x7F] = schema;
    
    printf("Registered schema for message ID 0x%02X (%zu fields, %zu bytes)\n", 
           message_id & 0x7F, schema.fields.size(), schema.get_total_size());
    
    return true;
}

const SerialLoggerSchema* SerialLoggerEncoder::get_schema(uint8_t message_id) const {
    auto it = schemas.find(message_id & 0x7F);
    if (it != schemas.end()) {
        return &it->second;
    }
    return nullptr;
}

size_t SerialLoggerEncoder::serialize_value(uint8_t* buffer, const SerialLoggerField& field, double value) const {
    size_t size = field.get_size();
    
    switch (field.type) {
        case SerialLoggerTypeCode::U8: {
            uint8_t v = (uint8_t)value;
            buffer[0] = v;
            break;
        }
        case SerialLoggerTypeCode::U16: {
            uint16_t v = (uint16_t)value;
            buffer[0] = v & 0xFF;
            buffer[1] = (v >> 8) & 0xFF;
            break;
        }
        case SerialLoggerTypeCode::U32: {
            uint32_t v = (uint32_t)value;
            buffer[0] = v & 0xFF;
            buffer[1] = (v >> 8) & 0xFF;
            buffer[2] = (v >> 16) & 0xFF;
            buffer[3] = (v >> 24) & 0xFF;
            break;
        }
        case SerialLoggerTypeCode::U64: {
            uint64_t v = (uint64_t)value;
            for (int i = 0; i < 8; i++) {
                buffer[i] = (v >> (i * 8)) & 0xFF;
            }
            break;
        }
        
        case SerialLoggerTypeCode::I8: {
            int8_t v = (int8_t)value;
            buffer[0] = v;
            break;
        }
        case SerialLoggerTypeCode::I16: {
            int16_t v = (int16_t)value;
            buffer[0] = v & 0xFF;
            buffer[1] = (v >> 8) & 0xFF;
            break;
        }
        case SerialLoggerTypeCode::I32: {
            int32_t v = (int32_t)value;
            buffer[0] = v & 0xFF;
            buffer[1] = (v >> 8) & 0xFF;
            buffer[2] = (v >> 16) & 0xFF;
            buffer[3] = (v >> 24) & 0xFF;
            break;
        }
        case SerialLoggerTypeCode::I64: {
            int64_t v = (int64_t)value;
            for (int i = 0; i < 8; i++) {
                buffer[i] = (v >> (i * 8)) & 0xFF;
            }
            break;
        }
        
        case SerialLoggerTypeCode::F32: {
            float v = (float)value;
            uint32_t bits;
            memcpy(&bits, &v, sizeof(float));
            buffer[0] = bits & 0xFF;
            buffer[1] = (bits >> 8) & 0xFF;
            buffer[2] = (bits >> 16) & 0xFF;
            buffer[3] = (bits >> 24) & 0xFF;
            break;
        }
        case SerialLoggerTypeCode::F64: {
            uint64_t bits;
            memcpy(&bits, &value, sizeof(double));
            for (int i = 0; i < 8; i++) {
                buffer[i] = (bits >> (i * 8)) & 0xFF;
            }
            break;
        }
        
        case SerialLoggerTypeCode::Q3_4: { QNumber3_4 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q4_4: { QNumber4_4 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q7_1: { QNumber7_1 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q1_7: { QNumber1_7 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q6_2: { QNumber6_2 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q2_6: { QNumber2_6 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q7_8: { QNumber7_8 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q8_8: { QNumber8_8 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q15_1: { QNumber15_1 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q1_15: { QNumber1_15 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q9_6: { QNumber9_6 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q10_6: { QNumber10_6 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q12_12: { QNumber12_12 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q16_8: { QNumber16_8 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q8_16: { QNumber8_16 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q15_16: { QNumber15_16 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q16_16: { QNumber16_16 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q24_8: { QNumber24_8 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q8_24: { QNumber8_24 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q31_32: { QNumber31_32 q(value); q.toBytes(buffer); break; }
        case SerialLoggerTypeCode::Q32_32: { QNumber32_32 q(value); q.toBytes(buffer); break; }
        
        default:
            return 0;
    }
    
    return size;
}

size_t SerialLoggerEncoder::encode_schema_packet(uint8_t message_id, const char* schema_str, uint8_t* output) {
    uint8_t raw_buffer[MAX_DATA_BYTES + 3];
    
    raw_buffer[0] = message_id | 0x80;
    
    size_t schema_len = strlen(schema_str);
    if (schema_len > MAX_DATA_BYTES - 2) {
        schema_len = MAX_DATA_BYTES - 2;
    }
    memcpy(raw_buffer + 1, schema_str, schema_len);
    
    size_t raw_len = 1 + schema_len;
    
    CRC16::append(raw_buffer, raw_len);
    raw_len += 2;
    
    size_t cobs_len = COBS::encode(raw_buffer, raw_len, output);
    output[cobs_len] = 0x00;
    
    return cobs_len + 1;
}

size_t SerialLoggerEncoder::encode_data_packet(uint8_t message_id, const double* values, size_t numValues, uint8_t* output) {
    const SerialLoggerSchema* schema = get_schema(message_id);
    if (!schema) {
        printf("No schema for message ID 0x%02X\n", message_id);
        return 0;
    }
    
    if (numValues != schema->fields.size()) {
        printf("Value count mismatch: got %zu, expected %zu\n", numValues, schema->fields.size());
        return 0;
    }
    
    uint8_t raw_buffer[MAX_DATA_BYTES + 3];
    
    raw_buffer[0] = message_id & 0x7F;
    
    size_t offset = 1;
    for (size_t i = 0; i < numValues; i++) {
        offset += serialize_value(raw_buffer + offset, schema->fields[i], values[i]);
    }
    
    CRC16::append(raw_buffer, offset);
    offset += 2;
    
    size_t cobs_len = COBS::encode(raw_buffer, offset, output);
    output[cobs_len] = 0x00;
    
    return cobs_len + 1;
}

// =============================================================================
// SerialLoggerDataBuilder Implementation
// =============================================================================

SerialLoggerEncoder::SerialLoggerDataBuilder::SerialLoggerDataBuilder(SerialLoggerEncoder* enc, uint8_t message_id) 
    : encoder(enc), offset(1), field_index(0) {
    
    schema = encoder->get_schema(message_id);
    if (!schema) {
        printf("No schema for message ID 0x%02X\n", message_id);
        return;
    }
    
    raw_buffer[0] = message_id & 0x7F;
}

SerialLoggerEncoder::SerialLoggerDataBuilder& SerialLoggerEncoder::SerialLoggerDataBuilder::add(uint8_t value) {
    if (!schema || field_index >= schema->fields.size()) return *this;
    
    const SerialLoggerField& field = schema->fields[field_index];
    offset += encoder->serialize_value(raw_buffer + offset, field, (double)value);
    field_index++;
    return *this;
}

SerialLoggerEncoder::SerialLoggerDataBuilder& SerialLoggerEncoder::SerialLoggerDataBuilder::add(uint16_t value) {
    if (!schema || field_index >= schema->fields.size()) return *this;
    
    const SerialLoggerField& field = schema->fields[field_index];
    offset += encoder->serialize_value(raw_buffer + offset, field, (double)value);
    field_index++;
    return *this;
}

SerialLoggerEncoder::SerialLoggerDataBuilder& SerialLoggerEncoder::SerialLoggerDataBuilder::add(uint32_t value) {
    if (!schema || field_index >= schema->fields.size()) return *this;
    
    const SerialLoggerField& field = schema->fields[field_index];
    offset += encoder->serialize_value(raw_buffer + offset, field, (double)value);
    field_index++;
    return *this;
}

SerialLoggerEncoder::SerialLoggerDataBuilder& SerialLoggerEncoder::SerialLoggerDataBuilder::add(uint64_t value) {
    if (!schema || field_index >= schema->fields.size()) return *this;
    
    const SerialLoggerField& field = schema->fields[field_index];
    
    if (field.type == SerialLoggerTypeCode::U64) {
        for (int i = 0; i < 8; i++) {
            raw_buffer[offset + i] = (value >> (i * 8)) & 0xFF;
        }
        offset += 8;
    } else {
        offset += encoder->serialize_value(raw_buffer + offset, field, (double)value);
    }
    
    field_index++;
    return *this;
}

SerialLoggerEncoder::SerialLoggerDataBuilder& SerialLoggerEncoder::SerialLoggerDataBuilder::add(int8_t value) {
    if (!schema || field_index >= schema->fields.size()) return *this;
    
    const SerialLoggerField& field = schema->fields[field_index];
    offset += encoder->serialize_value(raw_buffer + offset, field, (double)value);
    field_index++;
    return *this;
}

SerialLoggerEncoder::SerialLoggerDataBuilder& SerialLoggerEncoder::SerialLoggerDataBuilder::add(int16_t value) {
    if (!schema || field_index >= schema->fields.size()) return *this;
    
    const SerialLoggerField& field = schema->fields[field_index];
    offset += encoder->serialize_value(raw_buffer + offset, field, (double)value);
    field_index++;
    return *this;
}

SerialLoggerEncoder::SerialLoggerDataBuilder& SerialLoggerEncoder::SerialLoggerDataBuilder::add(int32_t value) {
    if (!schema || field_index >= schema->fields.size()) return *this;
    
    const SerialLoggerField& field = schema->fields[field_index];
    offset += encoder->serialize_value(raw_buffer + offset, field, (double)value);
    field_index++;
    return *this;
}

SerialLoggerEncoder::SerialLoggerDataBuilder& SerialLoggerEncoder::SerialLoggerDataBuilder::add(int64_t value) {
    if (!schema || field_index >= schema->fields.size()) return *this;
    
    const SerialLoggerField& field = schema->fields[field_index];
    
    if (field.type == SerialLoggerTypeCode::I64) {
        for (int i = 0; i < 8; i++) {
            raw_buffer[offset + i] = (value >> (i * 8)) & 0xFF;
        }
        offset += 8;
    } else {
        offset += encoder->serialize_value(raw_buffer + offset, field, (double)value);
    }
    
    field_index++;
    return *this;
}

SerialLoggerEncoder::SerialLoggerDataBuilder& SerialLoggerEncoder::SerialLoggerDataBuilder::add(float value) {
    if (!schema || field_index >= schema->fields.size()) return *this;
    
    const SerialLoggerField& field = schema->fields[field_index];
    offset += encoder->serialize_value(raw_buffer + offset, field, (double)value);
    field_index++;
    return *this;
}

SerialLoggerEncoder::SerialLoggerDataBuilder& SerialLoggerEncoder::SerialLoggerDataBuilder::add(double value) {
    if (!schema || field_index >= schema->fields.size()) return *this;
    
    const SerialLoggerField& field = schema->fields[field_index];
    offset += encoder->serialize_value(raw_buffer + offset, field, value);
    field_index++;
    return *this;
}

size_t SerialLoggerEncoder::SerialLoggerDataBuilder::send(uint8_t* output) {
    if (!schema) {
        printf("Invalid builder: no schema\n");
        return 0;
    }
    
    if (field_index != schema->fields.size()) {
        printf("Field count mismatch: added %zu, expected %zu\n", 
               field_index, schema->fields.size());
        return 0;
    }
    
    CRC16::append(raw_buffer, offset);
    offset += 2;
    
    size_t cobs_len = COBS::encode(raw_buffer, offset, output);
    
    output[cobs_len] = 0x00;
    
    return cobs_len + 1;
}
