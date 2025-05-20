#include "05_WIFI.h"    //
#include "WiFi.h"       // Thư viện WiFi của ESP32.
#include <WebServer.h>  //
#include <WiFiAP.h>     //
#include <EEPROM.h>     // Thư viện để lưu dữ liệu thông tin WiFi vào bộ nhớ ROM.
#include "WEBConfig.h"  // Giao diện HTML trang cấu hình WiFi cho board.

WebServer server(80);
#define EEPROM_SSID 415
#define EEPROM_PASS 447

IPAddress ip_ap(192, 168, 9, 1);
IPAddress gateway_ap(192, 168, 9, 1);
IPAddress subnet_ap(255, 255, 255, 0);

String WIFI::TenWiFi = "";
String WIFI::MatKhauWiFi = "";
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
String WIFI::LaySoMAC(void) {
#pragma region LaySoMAC
  //===========================================================================
  //---------------------------------------------------------------
  // Lấy số MAC của ESP32, đây là số IMEI của board.
  //---------------------------------------------------------------
  String MAC;
  MAC = WiFi.macAddress();
  MAC.replace(":", "");


#ifdef debug
  Serial.println("");
  Serial.print("MAC: ");
  Serial.println(MAC);
#endif
  //---------------------------------------------------------------

  return MAC;
//===========================================================================
#pragma endregion LaySoMAC
}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
// Kết nối WiFi.
void WIFI::KetNoiWiFi(int ThoiGianChoKetNoi) {
#pragma region KetNoiWiFi
  //===========================================================================
  // Xóa các cấu hình WiFi cũ đã lưu trước đó trong ESP.
  //ESP.eraseConfig();

  // Chuyển sang MODE STA (Stations) để dừng phát Access Point.
  WiFi.mode(WIFI_STA);
  delay(250);

  // Bắt đầu kết nối WiFi.
  WiFi.begin(TenWiFi, MatKhauWiFi);

#ifdef debug
  Serial.print("Đang kết nối WiFi... ");
#endif

  // Kiểm tra kết nối WiFi
  //---------------------------------------------------------------
  uint16_t count = 0;
  while (count < ThoiGianChoKetNoi) {
    if (WiFi.status() == WL_CONNECTED) {

#ifdef debug
      Serial.println("Kết nối WiFi thành công!");
#endif
      break;
    }
    delay(1000);
    count++;

#ifdef debug
    Serial.print(count);
    Serial.print(" > ");
#endif
  }

#ifdef debug
  if (count >= ThoiGianChoKetNoi) {
    Serial.println("");
    Serial.println("Đã hết thời gian, kết nối WiFi thất bại!");
  }
#endif
  //---------------------------------------------------------------
//===========================================================================
#pragma endregion KetNoiWiFi
}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
int WIFI::TinhDoManhCuaWiFi(void) {
#pragma region TinhDoManhCuaWiFi
  //===========================================================================
  if (WiFi.status() != WL_CONNECTED)
    return -1;


  int dBm = WiFi.RSSI();


#ifdef debug
  Serial.print("WiFi's RSSI: ");
  Serial.println(dBm);
#endif

  if (dBm <= -100)
    return 0;
  if (dBm >= -50)
    return 100;
  return 2 * (dBm + 100);
  //===========================================================================
#pragma endregion TinhDoManhCuaWiFi
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void WIFI::DocWiFiDaLuuTrongEEPROM(void) {
#pragma region DocWiFiDaLuuTrongEEPROM
  //===============================================================================
  if (EEPROM.read(EEPROM_SSID) != 0) {
    for (int i = EEPROM_SSID; i < EEPROM_PASS; ++i) {
      TenWiFi += char(EEPROM.read(i));
    }

    for (int i = EEPROM_PASS; i < 512; ++i) {
      MatKhauWiFi += char(EEPROM.read(i));
    }
    TenWiFi = TenWiFi.c_str();
    MatKhauWiFi = MatKhauWiFi.c_str();

#ifdef debug
    Serial.print("Tên WiFi: ");
    Serial.println(TenWiFi);
    Serial.print("Mật khẩu: ");
    Serial.println(MatKhauWiFi);
#endif
  }
  //===============================================================================
#pragma endregion DocWiFiDaLuuTrongEEPROM
}
String WIFI::rssiToIcon(int rssi) {
  if (rssi >= -60) {
    return "w0";
  } else if (rssi >= -67) {
    return "w1";
  } else if (rssi >= -75) {
    return "w2";
  } else {
    return "w3";
  }
}
void WIFI::NgatKetNoiWiFi(void) {
  WiFi.disconnect(true);
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void WIFI::QuetTimWiFi(void) {
#pragma region QuetTimWiFi
  //===============================================================================
  WiFi.disconnect();
  int n = WiFi.scanNetworks();

  //---------------------------------------------------------------
  uint16_t count = 0;
  while (count < 10) {
    delay(100);
    count++;
    Serial.print(count);
    Serial.print(" > ");
  }
  //---------------------------------------------------------------

#ifdef debug
  if (n <= 0)
    Serial.println("Không tìm thấy WiFi nào.");
  else {
    Serial.print("Đã quét thấy ");
    Serial.print(n);
    Serial.println(" mạng WiFi");
    for (int i = 0; i < n; ++i) {
      wifi_auth_mode_t encryption = WiFi.encryptionType(i);
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((encryption == WIFI_AUTH_OPEN) ? " " : " *");
      delay(10);
    }
  }
  Serial.println("");
#endif

  String str = "";
  for (int i = 0; i < n; ++i) {
    // Print SSID and RSSI for each network found
    String tenWiFi = WiFi.SSID(i);
    String ap_class = "";
    String icon = WIFI::rssiToIcon(WiFi.RSSI(i));
    str += "<div class='ape'><div class='" + icon + "'><div>" + tenWiFi + "</div></div></div>";
  }
  delay(100);
  server.send(200, "text/html", str);
  //===============================================================================
#pragma endregion QuetTimWiFi
}
void WIFI::ServerON(void) {
#pragma region ServerON
  server.on("/", WIFI::TrangChu);                          // Mở trang web để cấu hình WiFi cho board
  server.on("/style.css", WIFI::StyleCSS);                 // Gửi style và css qua file style.css
  server.on("/code.js", WIFI::CodeJS);                     // Gửi code js qua file code.js
  server.on("/LuuWiFiVaoEEPROM", WIFI::LuuWiFiVaoEEPROM);  // Gọi hàm khi user bấm nút LuuWiFiVaoEEPROM
  server.on("/QuetTimWiFi", WIFI::QuetTimWiFi);            // Gọi hàm khi user bấm nút QuetTimWiFi

  server.begin();  // Khởi động server
#pragma endregion ServerON
}
void WIFI::ThietLapAP(void) {
#pragma region ThietLapAP
  //===============================================================================
  if (this->DaBatAP == false) {
    // Bật cờ này lên để k phải gọi lại tác vụ thiết lập AP này nhiều lần.
    this->DaBatAP = true;

    // test (khó vào AP nên test)
    WiFi.disconnect(true);

    // Chuyển sang mode phát Access Point của ESP32.
    // Lưu ý phải là mode WIFI_AP_STA để lúc này ESP32 vừa
    // có thể phát Access Point vừa là Station kết nối WiFi
    WiFi.mode(WIFI_AP_STA);

    WiFi.softAPConfig(ip_ap, gateway_ap, subnet_ap);
    WiFi.softAP(this->AP_SSID, this->AP_PASS);
    ServerON();

#ifdef debug
    Serial.println("Đang thiết lập Access Point...");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("WiFi do ESP32 phát ra: ");
    Serial.print(this->AP_SSID);
    Serial.print("|Mật khẩu: ");
    Serial.println(this->AP_PASS);
#endif
  } else {
#ifdef debug
    Serial.println("Access Point đã được thiết lập!");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("WiFi do ESP32 phát ra: ");
    Serial.print(this->AP_SSID);
    Serial.print("|Mật khẩu: ");
    Serial.println(this->AP_PASS);
#endif
  }
  //===============================================================================
#pragma endregion ThietLapAP
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void WIFI::TrangChu(void) {
  String s = FPSTR(MainpageHTML);
  server.send(200, "text/html", s);
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void WIFI::StyleCSS(void) {
  String s = FPSTR(style_css);
  server.send(200, "text/css", s);
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void WIFI::CodeJS(void) {
  String s = FPSTR(code_js);
  server.send(200, "text/js", s);
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void WIFI::LuuWiFiVaoEEPROM(void) {
#pragma region LuuWiFiVaoEEPROM
  //===============================================================================
  // Lấy tên WiFi và mật khẩu từ phương thức GET ở hàm LuuWiFiVaoEEPROM()
  // trong file WEBConfig.h
  WIFI::TenWiFi = server.arg("TenWiFi");
  WIFI::MatKhauWiFi = server.arg("MatKhauWiFi");

  // Trước tiên cần xóa trắng thông tin WiFi đã lưu trước đó.
  for (int i = EEPROM_SSID; i < 512; ++i) {
    EEPROM.write(i, 0);
  }

  Serial.println("Lưu tên WiFi vào EEPROM...");
  for (int i = 0; i < TenWiFi.length(); ++i) {
    EEPROM.write(EEPROM_SSID + i, TenWiFi[i]);
    delay(10);
    Serial.print("Đã lưu: ");
    Serial.println(TenWiFi[i]);
  }

  Serial.println("Lưu mật khẩu WiFi vào EEPROM...");
  for (int i = 0; i < MatKhauWiFi.length(); ++i) {
    EEPROM.write(EEPROM_PASS + i, MatKhauWiFi[i]);
    delay(10);
    Serial.print("Đã lưu: ");
    Serial.println(MatKhauWiFi[i]);
  }

  // The write command does not write directly to EEPROM. It just prepares an array
  // of bytes which must then be saved in the memory area using the commit() command.
  EEPROM.commit();
  delay(500);

#ifdef debug
  Serial.println("Đã lưu WiFi vào EEPROM!");
  Serial.print("Tên WiFi: ");
  Serial.println(TenWiFi);
  Serial.print("Mật khẩu: ");
  Serial.println(MatKhauWiFi);
#endif

  String str;
  str = "Lưu thành công WiFi: ";
  str += TenWiFi;
  str += "|Mật khẩu: ";
  str += MatKhauWiFi;
  server.send(200, "text/html", str);

  // Kết nối WiFi
  WiFi.begin(TenWiFi, MatKhauWiFi);
  //===============================================================================
#pragma endregion LuuWiFiVaoEEPROM
}
void WIFI::KiemTraKetNoiWiFi(void) {
#pragma region KiemTraKetNoiWiFi
  //===============================================================================
  // #ifdef debug
  //   Serial.println("Kiểm tra kết nối WiFi...");
  //   Serial.print("WiFi.softAPIP:");
  //   Serial.println(WiFi.softAPIP().toString());
  //   Serial.print("ip_ap:");
  //   Serial.println(ip_ap.toString());
  //   Serial.print("Độ mạnh của WiFi:");
  //   Serial.println(WIFI::TinhDoManhCuaWiFi());
  // #endif

  if (WiFi.status() != WL_CONNECTED) {
    // Nếu ESP mất kết nối WiFi thì sẽ tự động phát Access Point (AP)
    // để điện thoại có thể quét được WiFi phát ra của board, nhờ đó user
    // có thể kết nối web cấu hình WiFi của board để tiến hành cài đặt thông số WiFi mới (nếu muốn).
    ThietLapAP();


  } else {
    // Trường hợp có kết nối WiFi ok, thì kiểm tra xem trước đó
    // cờ bật Access Point có bật không (true), nếu trước đó cờ có bật
    // thì sẽ chuyển mode WiFi của ESP32 sang mode STA, lúc này ESP32
    // sẽ kết nối WiFi với mạng WiFi đang có trong nhà.
    if (this->DaBatAP == true) {
      // Chuyển sang MODE STA (Stations) để dừng phát Access Point
      WiFi.mode(WIFI_STA);
      Serial.println("Kết nối WiFi OK. Tắt chế độ phát Access Point.");

      // Tắt cờ trạng thái đang thiết lập AP.
      this->DaBatAP = false;
    }
  }

  if (this->DaBatAP == true) {
    // Nếu như mà kết nối WiFi bị mất thì board sẽ phát ra WiFi của board,
    // tức là phát access point (AP), lúc này gọi hàm handleClient() để server
    // lắng nghe các thao tác từ trang web dùng để config WiFi.
    // CHÚ Ý: Luôn luôn phải gọi hàm này thì mới mở được
    // trang web WEBConfig mỗi khi board phát AP.
    server.handleClient();
  }
  //===============================================================================
#pragma endregion KiemTraKetNoiWiFi
}