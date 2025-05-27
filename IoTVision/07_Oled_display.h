#ifndef _07_OLED_DISPLAY_H
#define _07_OLED_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #define OLED_SDA 17
// #define OLED_SCL 16
// #define OLED_ADDRESS 0x3C

class OLED {
public:
  // Constructor với tham số custom
  OLED(TwoWire* wire, uint8_t sda, uint8_t scl, uint8_t width, uint8_t height, uint8_t address);

  // Hàm khởi tạo OLED
  bool begin();

  // Các hàm điều khiển OLED
  void clear();
  void displayText(const String& text, uint8_t x = 0, uint8_t y = 0, uint8_t size = 1);
  void displayData(double temp, double humidity);
  Adafruit_SSD1306* getDisplay();

  /*!
 @brief có thể sử dụng thư viện của Adafruit_SSD1306
 như sau
 OLED oled(&I2C_OLED, OLED_SDA, OLED_SCL, SCREEN_WIDTH, SCREEN_HEIGHT, OLED_ADDRESS);
 void setup(){
oled.begin();
}
 void main(){
  như 
  oled->getDisplay.setCursor(0,0);
 }
*/

private:
  Adafruit_SSD1306 _display;
  TwoWire* _wire;
  uint8_t _sda;
  uint8_t _scl;
  uint8_t _address;
};

#endif
