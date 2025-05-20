#include "03_Relay.h"


void Relay::KhoiTaoCacChan(void) {
  // Thiết lập mode cho các pin.
  pinMode(_pinRelay1, OUTPUT);
  pinMode(_pinRelay2, OUTPUT);
}
void Relay::OFFCacRole() {
  digitalWrite(_pinRelay1, _OFF);
  digitalWrite(_pinRelay2, _OFF);
  this->K1 = _OFF;
  this->K2 = _OFF;
}


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//================== Begin: ĐÓNG NGẮT RƠLE BẰNG TAY =======================//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
#pragma region ĐÓNG NGẮT RƠLE BẰNG TAY
bool Relay::ONOFFBangTayK1(int K1) {
#pragma region ONOFFBangTayK1            
  // Chỉ thực thi lệnh điều khiển khi có sự thay đổi trạng thái.
  if (K1 != this->K1) {
    // Cập nhật trạng thái ON/OFF hiện tại của các kênh vào
    // bảng dữ liệu & dữ liệu hiển thị online của board trên database
    //-----------------------------------------------------------------
    this->K1 = K1;
    //-----------------------------------------------------------------


#ifdef debug
    Serial.print("K1 đã được cập nhật: ");
    Serial.println(this->K1);
#endif


    // Đóng/ngắt điện cho thiết bị K1 khi và chỉ khi
    // chế độ điều khiển là bằng tay (Manual).
    if (this->K1 == 0)
      this->K1 = _OFF;
    else if (this->K1 == 1)
      this->K1 = _ON;
    digitalWrite(_pinRelay1, this->K1);


    // Trả về true nếu có thực thi đóng/ngắt rơle.
    return true;
  }


  // Trả về false nếu không có thực thi đóng/ngắt rơle.
  return false;
#pragma endregion ONOFFBangTayK1
}


bool Relay::ONOFFBangTayK2(int K2) {
#pragma region ONOFFBangTayK2
#ifdef debug
  Serial.print("K2 cũ: ");
  Serial.println(this->K2);
  Serial.print("K2 mới: ");
  Serial.println(K2);
#endif


  //=========================================================================
  //------- Begin: Thực thi khi chế độ điều khiển là MANUAL cho K2 --------//   
  //=========================================================================
  // Chỉ thực thi lệnh điều khiển khi có sự thay đổi trạng thái.
  if (K2 != this->K2) {
    // Cập nhật trạng thái ON/OFF hiện tại của các kênh vào
    // bảng dữ liệu & dữ liệu hiển thị online của board trên database
    //-----------------------------------------------------------------
    this->K2 = K2;
    //-----------------------------------------------------------------


#ifdef debug
    Serial.print("K2 đã được cập nhật: ");
    Serial.println(this->K2);
#endif


    // Đóng/Ngắt điện cho thiết bị K2 khi và chỉ khi
    // chế độ điều khiển là bằng tay (Manual).
    if (this->K2 == 0)
      this->K2 = _OFF;
    else if (this->K2 == 1)
      this->K2 = _ON;
    digitalWrite(_pinRelay2, this->K2);


    // Trả về true nếu có thực thi đóng/ngắt rơle.
    return true;
  }     
  //=========================================================================
  //------- End: Thực thi khi chế độ điều khiển là MANUAL cho K2 ----------//   
  //=========================================================================


  // Trả về false nếu không có thực thi đóng/ngắt rơle.
  return false;
#pragma endregion ONOFFBangTayK2
}
#pragma endregion ĐÓNG NGẮT RƠLE BẰNG TAY
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//================== End: ĐÓNG NGẮT RƠLE BẰNG TAY =========================//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//============= Begin: BOARD LÀM VIỆC Ở CHẾ ĐỘ TỰ ĐỘNG ====================//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
#pragma region BOARD LÀM VIỆC Ở CHẾ ĐỘ TỰ ĐỘNG
bool Relay::TuDongDongNgatKenh1TheoNhietDo(double NhietDo, double MIN, double MAX) {
  //=======================================================================//
  //--------- Begin: Đóng ngắt kênh 1 dựa theo cảm biến nhiệt độ ----------//
  //=======================================================================//
  if (NhietDo < MIN) {
    // Chỉ thay đổi khi trạng thái trước đó của K1 là ON
    if (this->K1 == 1) {
      this->K1 = 0;
      digitalWrite(_pinRelay1, _OFF);  // Rơle 1 OFF


      // Trả về true nếu có thực thi đóng/ngắt rơle.
      return true;
    }
  } else if (NhietDo > MAX) {
    // Chỉ thay đổi khi trạng thái trước đó của K1 là OFF
    if (this->K1 == 0) {
      this->K1 = 1;
      digitalWrite(_pinRelay1, _ON);  // Rơle 1 ON


      // Trả về true nếu có thực thi đóng/ngắt rơle.
      return true;
    }
  }
  //=======================================================================//
  //--------- End: Đóng ngắt kênh 1 dựa theo cảm biến nhiệt độ ------------//
  //=======================================================================//


  // Trả về false nếu không có thực thi đóng/ngắt rơle.
  return false;
}
bool Relay::TuDongDongNgatKenh2TheoDoAm(double DoAm, double MIN, double MAX) {
  //=======================================================================//
  //--------- Begin: Đóng ngắt kênh 2 dựa theo cảm biến độ ẩm -------------//
  //=======================================================================//
  if (DoAm < MIN) {
    // Chỉ thay đổi khi trạng thái trước đó của K2 là OFF
    if (this->K2 == 0) {
      this->K2 = 1;
      digitalWrite(_pinRelay2, _ON);  // Rơle 2 ON


      // Trả về true nếu có thực thi đóng/ngắt rơle.
      return true;
    }
  } else if (DoAm > MAX) {
    // Chỉ thay đổi khi trạng thái trước đó của K2 là ON
    if (this->K2 == 1) {
      this->K2 = 0;
      digitalWrite(_pinRelay2, _OFF);  // Rơle 2 OFF


      // Trả về true nếu có thực thi đóng/ngắt rơle.
      return true;
    }
  }
  //=======================================================================//
  //--------- End: Đóng ngắt kênh 2 dựa theo cảm biến độ ẩm ---------------//
  //=======================================================================//


  // Trả về false nếu không có thực thi đóng/ngắt rơle.
  return false;
}
#pragma endregion BOARD LÀM VIỆC Ở CHẾ ĐỘ TỰ ĐỘNG
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//============= End: BOARD LÀM VIỆC Ở CHẾ ĐỘ TỰ ĐỘNG ======================//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
