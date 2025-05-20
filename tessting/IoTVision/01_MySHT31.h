#ifndef _SHT31_h
#define _SHT31_h

#include "IoTVision.h"  // Có define debug để bật/tắt các debug ra Serial.

// Thư viện cho cảm biến nhiệt độ và độ ẩm SHT3x (gồm SHT30 và SHT31).
#include "SHT31.h"

//-----------------------------------------------------------------
// Specify data and clock connections and instantiate SHT31 object
// Giao tiếp giữa ESP32 với SHT31 là giao thức I2C.
//-----------------------------------------------------------------
#define SHT31_ADDRESS 0x44
#define dataPin 19   //Chân SDA
#define clockPin 18  //Chân SCL
//-----------------------------------------------------------------

class SHT3x {
private:
  SHT31 sht31;

public:
  double NhietDo;
  double DoAm;

public:
  void KhoiTaoSHT31(void);
  void DocCamBienNhietDoVaDoAmSHT31(void);
};

#endif
