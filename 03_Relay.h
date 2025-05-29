#ifndef _Relay_h
#define _Relay_h

#include "IoTVision.h"     // Có define debug để bật/tắt các debug ra Serial.
#include <Arduino.h>       // Để khai báo kiểu String
#include <Arduino_JSON.h>  // Thư viện xử lý dữ liệu kiểu JSON

// Thiết lập các pin
//---------------------------------------------------------------------------
#define _pinRelay1 15 // Đóng ngắt kênh K1.
#define _pinRelay2 21 // Đóng ngắt kênh K2.
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#define _AUTO 1    // Chế độ làm việc tự động.
#define _MANUAL 0  // Chế độ làm việc bằng tay
#define _ON 1      // ON thiết bị.
#define _OFF 0     // OFF thiết bị.
//---------------------------------------------------------------------------

class Relay {
public:
  int K1;   // Relay điều khiển ON/OFF K1.
  int K2;   // Relay điều khiển ON/OFF K2.
  int MODE; // Chế độ AUTO/MAN điều khiển tự động hay bằng tay.

public:
  void KhoiTaoCacChan(void);
  void OFFCacRole(void);

  bool ONOFFBangTayK1(int K1);
  bool ONOFFBangTayK2(int K2);
  bool TuDongDongNgatKenh1TheoNhietDo(double NhietDo, double MIN, double MAX);
  bool TuDongDongNgatKenh2TheoDoAm(double DoAm, double MIN, double MAX);
};

#endif
