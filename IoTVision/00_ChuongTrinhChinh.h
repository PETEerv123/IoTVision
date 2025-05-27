#ifndef _ChuongTrinhChinh_h
#define _ChuongTrinhChinh_h

#include "IoTVision.h"  // Có define debug để bật/tắt các debug ra Serial.
#include <Arduino.h>
#include <Arduino_JSON.h>  // Thư viện xử lý dữ liệu kiểu JSON
#include <Wire.h>          // Để kết nối I2C với mô-đun RTC (thời gian thực),
                           // mô-đun đọc cảm biến nhiệt độ & độ ẩm SHT3x.

// #define pinSCL 18 // Chân SCL của I2C nối cảm biến nhiệt độ & độ ẩm SHT3x
// #define pinSDA 19 // Chân SDA của I2C nối cảm biến nhiệt độ & độ ẩm SHT3x
#define SHT31_ADDRESS 0x44
#define SHT31_SDA 19
#define SHT31_SCL 18
//
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 17
#define OLED_SCL 16
#define OLED_ADDRESS 0x3C

#define SW1 34  //
#define SW2 35
#define SW3 13
#define SW4 14



void KhoiTao(void);
void ChayChuongTrinhChinh(void);
void ThucThiTacVuTheoFLAG(void);
void POSTDuLieuVeCloudDeHienThiTrenAPP(void);


// Các hàm thực thi tác vụ theo CODE
//------------------------------------------------------
void CapNhatCODETrenServer(String code);
void LangNgheLenhAppGuiXuongBoard(void);
void ThucThiTacVuTheoCODE(void);


// Trả về true nếu có thay đổi trạng thái làm việc của các K1, K2
// Ngược lại sẽ trả về false.
bool BoardLamViec(void);
#endif
