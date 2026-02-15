#pragma once

#include <stdint.h>
#include "logger/packet.h"

#include "v5.h"

#define HANDSHAKE_ID 0xFF
#define HANDSHAKE_RETRY_MS 5000

class SerialLogger {
private:
    uint32_t port;
    SerialLoggerEncoder encoder;
    bool connected;
    uint32_t last_handshake_attempt_ms;
    uint8_t rx_buffer[16];
    size_t rx_buffer_len;
    
    void send_handshake() {
        while (vexGenericSerialReceiveAvail(port) > 0) {
            uint8_t dummy[64];
            vexGenericSerialReceive(port, dummy, sizeof(dummy));
        }
        rx_buffer_len = 0;
        
        uint8_t raw_buffer[3];
        raw_buffer[0] = HANDSHAKE_ID;
        CRC16::append(raw_buffer, 1);
        
        uint8_t cobs_buffer[8];
        size_t cobs_len = COBS::encode(raw_buffer, 3, cobs_buffer);
        cobs_buffer[cobs_len] = 0x00;
        
        vexGenericSerialTransmit(port, cobs_buffer, cobs_len + 1);
    }
    
    void process_incoming_handshake() {
      vexDelay(25);
        while (vexGenericSerialReceiveAvail(port) > 0) {
            int32_t c = vexGenericSerialReadChar(port);
            if (c < 0) break;
            
            if ((uint8_t)c == 0x00) {
                if (rx_buffer_len >= 2) {
                    uint8_t decoded[16];
                    size_t decoded_len = COBS::decode(rx_buffer, rx_buffer_len, decoded);
                    
                    if (decoded_len == 3 && decoded[0] == HANDSHAKE_ID) {
                        if (CRC16::verify(decoded, decoded_len)) {
                            connected = true;
                            return;
                        }
                    }
                }
                rx_buffer_len = 0;
            } else {
                if (rx_buffer_len < sizeof(rx_buffer)) {
                    rx_buffer[rx_buffer_len++] = (uint8_t)c;
                } else {
                    rx_buffer_len = 0;
                }
            }
        }
    }
    
public:
    SerialLogger(uint32_t port_index) 
        : port(port_index), connected(false), last_handshake_attempt_ms(0), rx_buffer_len(0) {
        vexGenericSerialEnable(port, 0);
        vexGenericSerialBaudrate(port, 921600);
    }
    
    void update() {
        if (connected) return;

        printf("logger not connected\n");
        
        uint32_t now = vexSystemTimeGet();
        
        if (last_handshake_attempt_ms == 0 || (now - last_handshake_attempt_ms >= HANDSHAKE_RETRY_MS)) {
            send_handshake();
            last_handshake_attempt_ms = now;
        }
        
          vexDelay(100);
          process_incoming_handshake();
    }
    
    bool is_connected() const {
        return connected;
    }
    
    // "temp:Q16_16,humidity:Q8_8,pressure:u32"
    bool register_schema(uint8_t message_id, const char* schema_str) {
        return encoder.register_schema(message_id, schema_str);
    }
    
    void send_schema(uint8_t message_id, const char* schema_str) {
        if (!connected) return;
        
        uint8_t packet[256];
        size_t length = encoder.encode_schema_packet(message_id, schema_str, packet);
        
        if (length > 0) {
            vexGenericSerialTransmit(port, packet, length);
        }
    }
    
    void log(uint8_t message_id, const double* values, size_t num_values) {
        if (!connected) return;
        
        uint8_t packet[256];
        size_t length = encoder.encode_data_packet(message_id, values, num_values, packet);
        
        if (length > 0) {
            vexGenericSerialTransmit(port, packet, length);
        }
    }
    
    class LogBuilder {
    private:
        SerialLogger* logger;
        SerialLoggerEncoder::SerialLoggerDataBuilder builder;
        
    public:
        LogBuilder(SerialLogger* log, SerialLoggerEncoder::SerialLoggerDataBuilder bldr) 
            : logger(log), builder(bldr) {}
        
        LogBuilder& add(uint8_t value) { builder.add(value); return *this; }
        LogBuilder& add(uint16_t value) { builder.add(value); return *this; }
        LogBuilder& add(uint32_t value) { builder.add(value); return *this; }
        LogBuilder& add(uint64_t value) { builder.add(value); return *this; }
        LogBuilder& add(int8_t value) { builder.add(value); return *this; }
        LogBuilder& add(int16_t value) { builder.add(value); return *this; }
        LogBuilder& add(int32_t value) { builder.add(value); return *this; }
        LogBuilder& add(int64_t value) { builder.add(value); return *this; }
        LogBuilder& add(float value) { builder.add(value); return *this; }
        LogBuilder& add(double value) { builder.add(value); return *this; }
        
        void send() {
            if (!logger->connected) return;
            
            uint8_t packet[256];
            size_t length = builder.send(packet);
            if (length > 0) {
                vexGenericSerialTransmit(logger->port, packet, length);
            }
        }
    };
    
    LogBuilder build(uint8_t message_id) {
        return LogBuilder(this, encoder.build(message_id));
    }
    
    void define_and_send_schema(uint8_t message_id, const char* schema_str) {
        if (register_schema(message_id, schema_str)) {
            send_schema(message_id, schema_str);
        }
    }
    
    bool has_schema(uint8_t message_id) const {
        return encoder.has_schema(message_id);
    }
    
    uint32_t get_port() const {
        return port;
    }
    
    int32_t get_write_free() const {
        return vexGenericSerialWriteFree(port);
    }
    
    void flush() {
        vexGenericSerialFlush(port);
    }
};


