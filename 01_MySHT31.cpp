#include "01_MySHT31.h"


SHT3x::SHT3x(TwoWire* wire, uint8_t sda, uint8_t scl, uint8_t address)
: _wire(wire), _sda(sda), _scl(scl), _address(address), sht31(address, wire) {}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//================ Begin: ĐỌC CẢM BIẾN NHIỆT ĐỘ & ĐỘ ẨM ===================//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void SHT3x::KhoiTaoSHT31(void) {
  // Cảm biến SHT31 đọc nhiệt độ và độ ẩm.
  // Chú ý: Khai báo sau các khai báo pinMode.
  //---------------------------------------------------------------
  _wire->begin(_sda, _scl);
  sht31.begin();
  //---------------------------------------------------------------
}
void SHT3x::DocCamBienNhietDoVaDoAmSHT31() {
  sht31.read();
  NhietDo = sht31.getTemperature();
  NhietDo = double(round(NhietDo * 100) / 100);  // will round to 2 decimal places i.e. 0.00
  DoAm = sht31.getHumidity();
  DoAm = double(round(DoAm * 100) / 100);  // will round to 2 decimal places i.e. 0.00

  // Trường hợp không có kết nối cảm biến thì giá trị trả về là 
  // nhiệt độ = 130 (hoặc -45) & // độ ẩm = 100, lúc này trả về -1 
  // cho cả 2 thông số nhiệt độ & độ ẩm để trên app biết mà hiển thị 
  // trạng thái không có cảm biến kết nối với board để cho user biết.
  if (NhietDo >= 130 || NhietDo <= -45) {
    NhietDo = -1;
    DoAm = -1;
  }

#ifdef debug
  Serial.print("Nhiệt độ: ");
  Serial.println(NhietDo);
  Serial.print("Độ ẩm: ");
  Serial.println(DoAm);
#endif
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//================ End: ĐỌC CẢM BIẾN NHIỆT ĐỘ & ĐỘ ẨM =====================//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
