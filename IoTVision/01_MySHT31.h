#ifndef _SHT31_h
#define _SHT31_h

#include "IoTVision.h"  // Có define debug để bật/tắt các debug ra Serial.

// Thư viện cho cảm biến nhiệt độ và độ ẩm SHT3x (gồm SHT30 và SHT31).
#include "SHT31.h"

//-----------------------------------------------------------------
// Specify data and clock connections and instantiate SHT31 object
// Giao tiếp giữa ESP32 với SHT31 là giao thức I2C.
//-----------------------------------------------------------------
// #define SHT31_ADDRESS 0x44
// #define SHT31_SDA 19
// #define SHT31_SCL 18
//-----------------------------------------------------------------



class SHT3x {
private:
    SHT31 sht31;
    TwoWire* _wire;
    uint8_t _sda;
    uint8_t _scl;
    uint8_t _address;

public:
    double NhietDo;
    double DoAm;

    // Constructor với tham số cấu hình
    SHT3x(TwoWire* wire, uint8_t sda, uint8_t scl, uint8_t address = 0x44);
    
    void KhoiTaoSHT31();
    void DocCamBienNhietDoVaDoAmSHT31();
};

#endif
