#ifndef _RTC_h
#define _RTC_h

#include "IoTVision.h"  // Có define debug để bật/tắt các debug ra Serial.
#include <DS3231.h>      // Thư viện lấy thời gian thực (RTC)
#include <HTTPClient.h>  // Cho phép khai báo kiểu HTTPClient

typedef struct {
  int Thu;
  int Ngay;
  int Thang;
  int Nam;
  int Gio;
  int Phut;
  int Giay;
} ThoiGian;

class RTC {
private:
  HTTPClient http;
 DS3231 ds;

public:
  ThoiGian dt;

public:
  void KhoiTaoServerNTP(void);

private:
  // Lấy RTC từ nguồn server NTP hay IoTVision
  bool LayRTCTuServerNTP(void);
  bool LayRTCTuServerIoTVision(void);
  bool LayRTCTuServer(void);

public:
  void CaiDatRTCChoDS3231(void);
  void LayRTCTuServerCaiDatChoDS3231(void);
  void LayRTCTuDS3231(void);
  String ChuanHoaChuoiRTCDeGuiVeServer(void);
};

#endif
