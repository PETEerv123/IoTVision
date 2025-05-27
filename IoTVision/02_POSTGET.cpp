#include "02_POSTGET.h"
#include "WiFi.h"       // Thư viện WiFi của ESP32.


#define URL_POSTDuLieuTuBoardLenServer "http://App.IoTVision.vn/api/BoardSTR423_DuLieu"
#define URL_GETLenhGuiXuongBoard "http://App.IoTVision.vn/api/BoardSTR423_DuLieuGuiXuongBoard?CheDo=1&key="
#define URL_CapNhatCODE "http://App.IoTVision.vn/api/BoardSTR423_DuLieuGuiXuongBoard"
// Thiết lập thời gian timeout.
#define timeout 1500 // 1,5s (có thể thay đổi con số này)
//=========================================================================//
// Ví dụ, DuLieu = "11100;23.5;82.9;19:53:13 02/08/2023"
// 11100 => K1 = 1, K2 = 1, K3 = 1, K4 = 0, MODE = 0 (MAN)
// Nhiệt độ = 23.5
// Độ ẩm = 82.9
// HH:MM:SS DD/MM/YYYY = 19:53:13 02/08/2023
//=========================================================================//
bool POSTGET::POSTDuLieuBoard(String ID, String DuLieu) {
  if (WiFi.status() == WL_CONNECTED) {
    try {
    	if (WiFi.status() == WL_CONNECTED) {
        this->http.setTimeout(timeout);  // Thiết lập thời gian timeout.
        this->http.begin(URL_POSTDuLieuTuBoardLenServer);
        this->http.addHeader("Content-Type", "application/json");
        String data = "{\"ID\":\"" + ID + "\",\"S\":\"" + DuLieu + "\"}";
        this->http.POST(data);  //Send the request
        this->http.end();
        return 1;
    	} else
        return 0;
    } catch (String error) {
      return 0;
    }
  }
}
//=========================================================================//
//=========================================================================//
String POSTGET::GETLenhGuiXuongBoard(String ID) {
  try {
    if (WiFi.status() == WL_CONNECTED) {
      String url = URL_GETLenhGuiXuongBoard + ID;
      this->http.setTimeout(timeout);  // Thiết lập thời gian timeout.
      this->http.begin(url);
      this->http.addHeader("Content-Type", "application/json");
#ifdef debug
      unsigned long ms = millis();
#endif
      int httpCodeGet = this->http.GET();
      if (httpCodeGet == HTTP_CODE_OK) {  // needs a 200 to continue...
        String data = this->http.getString();
        data.remove(0, 1);                  // Loại ký tự '[' tại đầu chuỗi.
        data.remove(data.length() - 1, 1);  // Loại ký tự ']' tại cuối chuỗi.
        this->http.end();


#ifdef debug
        Serial.print(millis() - ms);
        Serial.print("(ms) for GET ");
        Serial.println(data);
#endif


        return data;
      } else {
#ifdef debug
        Serial.println(this->http.errorToString(httpCodeGet).c_str());
#endif
        this->http.end();
        return "";
      }
    } else {
#ifdef debug
      Serial.println("Mất kết nối WIFI!");
#endif
      return "";
    }
  } catch (String error) {
    return "";
  }
}
//=========================================================================//
// Ví dụ, DuLieu = "1011"
// K1 = 1, MODE = 0 (MAN), CODE = 11
//=========================================================================//
void POSTGET::CapNhatCODETrongDatabaseTrenServer(String ID, String DuLieu) {
  if (WiFi.status() == WL_CONNECTED) {
    this->http.setTimeout(timeout);  // Thiết lập thời gian timeout.
    this->http.addHeader("Content-Type", "application/json");
    this->http.begin(URL_CapNhatCODE);
    this->http.addHeader("Content-Type", "application/json");


    String data = "{\"ID\":\"" + ID + "\",\"S\":\"" + DuLieu + "\"}";
    this->http.POST(data);  //Send the request
    this->http.end();
  }
}
