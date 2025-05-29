#include "07_Oled_display.h"



OLED::OLED(TwoWire* wire, uint8_t sda, uint8_t scl, uint8_t width, uint8_t height, uint8_t address)
: _display(width, height, wire), _wire(wire), _sda(sda), _scl(scl), _address(address) {}

bool OLED::begin() {
  _wire->begin(_sda, _scl);
  if(!_display.begin(SSD1306_SWITCHCAPVCC, _address)) {
    return false;
  }
  _display.setTextColor(SSD1306_WHITE);
  _display.clearDisplay();
  return true;
}

void OLED::clear() {
  _display.clearDisplay();
}

void OLED::displayText(const String &text, uint8_t x, uint8_t y, uint8_t size) {
  _display.setTextSize(size);
  _display.setCursor(x, y);
  _display.println(text);
  _display.display();
}

void OLED::displayData(double temp, double humidity) {
  _display.clearDisplay();
  _display.setCursor(0, 0);
  _display.print("Nhiet do: ");
  _display.println(temp);
  _display.print("Do am: ");
  _display.println(humidity);
  _display.display();
}

Adafruit_SSD1306* OLED::getDisplay() {
  return &_display;
}