#include "04_RTC.h"
#include "WiFi.h"       // Thư viện WiFi của ESP32.
#include <Arduino_JSON.h>  // Thư viện xử lý dữ liệu kiểu JSON

// Thư viện cho phép lấy thời gian thực chuẩn từ NTP Server.
#include <NTPClient.h>
#include <WiFiUdp.h>

// Nếu define NTPServer thì RTC sẽ được lấy từ nguồn server NTP.
//#define NTPServer

#define _INSIZE 512 // fixed max size for server response

//---------------------------------------------------------------------------
WiFiUDP ntpUDP; // Lấy thời gian thực từ NTP Server

// You can specify the time server pool and the offset, (in seconds)
// additionally you can specify the update interval (in milliseconds).
// GMT +7 = 25200
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 25200, 60000);
//---------------------------------------------------------------------------

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//—--------- BEGIN: Lấy RTC từ nguồn server NTP hay IoTVision  ------------//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void RTC::KhoiTaoServerNTP(void) {
  timeClient.begin();
}
// Lấy thời gian thực từ NTP Server.
//---------------------------------------------------------------------------
bool RTC::LayRTCTuServerNTP(void) {
  if (WiFi.status() == WL_CONNECTED) {

#ifdef debug
    Serial.println("Đang lấy RTC từ server NTP...");
#endif

    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();
    int currentSecond = timeClient.getSeconds();
    int currentweekDay = timeClient.getDay();

    //Get a time structure
    struct tm *ptm = gmtime((time_t *)&epochTime);
    int monthDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon + 1;
    int currentYear = ptm->tm_year + 1900;

    // Chuyển RTC dạng JSON và kiểu dữ liệu
    // tự định nghĩa ThoiGian.
    //------------------------------------------------------
    dt.Thu = currentweekDay;  // CN = 0, Thứ 2 = 1,...
    dt.Ngay = monthDay;
    dt.Thang = currentMonth;
    dt.Nam = currentYear;
    dt.Gio = currentHour;
    dt.Phut = currentMinute;
    dt.Giay = currentSecond;
    //------------------------------------------------------

#ifdef debug
    Serial.println("Thời gian trên NTP Server: " + String(dt.Thu) + " " + String(dt.Ngay) + 
"/" + String(dt.Thang) + "/" + String(dt.Nam) + " " + String(dt.Gio) + ":" + String(dt.Phut) +
":" + String(dt.Giay));
#endif

    return true;
  } else {
#ifdef debug
    Serial.println("Không có WiFi để kết nối vào server NTP để lấy RTC");
#endif
  }

  return false;
}
//---------------------------------------------------------------------------
// Lấy thời gian thực từ IoTVision's Server.
bool RTC::LayRTCTuServerIoTVision(void) {
  if (WiFi.status() == WL_CONNECTED) {

#ifdef debug
    Serial.println("Đang lấy RTC từ server IoTVision...");
#endif

    String url = "https://App.IoTVision.vn/api/ThoiGian";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int httpCodeGet = http.GET();
    if (httpCodeGet == HTTP_CODE_OK) {  // needs a 200 to continue...
      int z = http.getSize();           // how big is the payload?
      if (z < _INSIZE) { // payload must fit in the available space.
        String thoigianthuc = http.getString();

#ifdef debug                      
        Serial.println("--------------------------------------------------");
        Serial.println("Thời thực lấy từ server IoTVision: ");
        Serial.println(thoigianthuc);
        Serial.println("--------------------------------------------------");       
#endif

        if (thoigianthuc.length() > 0) {
          // Chuyển chuỗi thoigianthuc sang dạng JSON
          JSONVar RTC = JSON.parse(thoigianthuc);

          // Chuyển RTC dạng JSON và kiểu dữ liệu
          // tự định nghĩa ThoiGian.
          //------------------------------------------------------
          dt.Thu = (int)RTC["Thu"];  // CN = 0, Thứ 2 = 1,...
          dt.Ngay = (int)RTC["Ngay"];
          dt.Thang = (int)RTC["Thang"];
          dt.Nam = (int)RTC["Nam"];
          dt.Gio = (int)RTC["Gio"];
          dt.Phut = (int)RTC["Phut"];
          dt.Giay = (int)RTC["Giay"];

          // Lấy 2 số cuối của năm, vdu: 2020 thì chỉ lấy số 20.
          dt.Nam -= 2000;
          //------------------------------------------------------

          http.end();
          return true;
        }
      }
    }
    http.end();
  } else {
#ifdef debug
    Serial.println("Không có WiFi để kết nối vào server IoTVision để lấy RTC");
#endif
  }

  return false;
}
//---------------------------------------------------------------------------
bool RTC::LayRTCTuServer(void) {
#ifdef NTPServer
  // Lấy RTC từ nguồn server NTP.
  return LayRTCTuServerNTP();
#else

  // Lấy RTC từ nguồn server IoTVision
  return LayRTCTuServerIoTVision();
#endif
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//—--------- End: Lấy RTC từ nguồn server NTP hay IoTVision  --------------//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//—----------- BEGIN: Lấy thời gian thực từ mô-đun RTC  —------------------//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
// Cài đặt thời gian cho DS3231.
void RTC::CaiDatRTCChoDS3231(void) {
  ds.setClockMode(false);  // set to 24h
  ds.setYear(dt.Nam);
  ds.setMonth(dt.Thang);
  ds.setDate(dt.Ngay);
  ds.setDoW(dt.Thu);  // CN = 0, Thứ 2 = 1,...
  ds.setHour(dt.Gio);
  ds.setMinute(dt.Phut);
  ds.setSecond(dt.Giay);
}
// Lấy thời gian thực từ server rồi cập nhật thời gian thực đó cho mô-đun RTC.
void RTC::LayRTCTuServerCaiDatChoDS3231(void) {
  if (LayRTCTuServer() == true)
    CaiDatRTCChoDS3231();  // Cài đặt giá trị RTC cho mô-đun RTC DS3231.
}
// Lấy thời gian thực từ mô-đun RTC.
void RTC::LayRTCTuDS3231(void) {
  bool century = false;
 bool h12Flag;
  bool pmFlag;
  dt.Thu = ds.getDoW();
  dt.Ngay = ds.getDate();
  dt.Thang = ds.getMonth(century);
  dt.Nam = ds.getYear();
  dt.Gio = ds.getHour(h12Flag, pmFlag);
  dt.Phut = ds.getMinute();
  dt.Giay = ds.getSecond();

  // Cộng thêm 2000 để ra đúng số năm, vdu: 20 + 2000 = 2020: năm 2020
  dt.Nam += 2000;
}

String RTC::ChuanHoaChuoiRTCDeGuiVeServer(void) {
  // Thêm dữ liệu thời gian RTC của DS1307 trên board gửi về server để có thể
  // đối chiếu kịp thời phát hiện sai lệch thời gian giữa board và server, từ
  // đó user có thể thực hiện update lại RTC cho DS1307 thông qua tính năng
  // cập nhật thông số board trên app.
  String Gio = String(dt.Gio);
  String Phut = String(dt.Phut);
  String Giay = String(dt.Giay);
  String Ngay = String(dt.Ngay);
  String Thang = String(dt.Thang);
  String Nam = String(dt.Nam);
  if (dt.Gio < 10) Gio = "0" + Gio;
  if (dt.Phut < 10) Phut = "0" + Phut;
  if (dt.Giay < 10) Giay = "0" + Giay;
  if (dt.Ngay < 10) Ngay = "0" + Ngay;
  if (dt.Thang < 10) Thang = "0" + Thang;

  String RTC = Gio + ":" + Phut + ":" + Giay + " " + Ngay + "/" + Thang + "/" + Nam;

#ifdef debug
  Serial.print("Thời gian RTC trên DS3231 đã chuẩn hóa: ");
  Serial.println(RTC);
#endif

  return RTC;
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//—----------- END: Lấy thời gian thực từ mô-đun RTC  —--------------------//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
