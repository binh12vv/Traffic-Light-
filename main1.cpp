#include <Arduino.h>
#define BLYNK_TEMPLATE_ID "TMPL6OirvDI4E"
#define BLYNK_TEMPLATE_NAME "traffic light"
#define BLYNK_AUTH_TOKEN "6RcSO1QPMu6cr_nO9gjMF9sjZcaUwzyX"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial

// Thông tin WiFi
char ssid[] = "Iphone";
char pass[] = "12345678";

// gán chân GPIO cho từng LED
const int red_LED1 = 14;      // LED đỏ 1
const int yellow_LED1 = 12;   // LED vàng 1
const int green_LED1 = 13;    // LED xanh 1
const int red_LED2 = 25;      // LED đỏ 2
const int yellow_LED2 = 26;   // LED vàng 2
const int green_LED2 = 27;    // LED xanh 2

// khai báo biến thời gian sáng (ms)
int time_red_1 = 30000;  // Thời gian sáng LED đỏ 1 
int time_yellow_1 = 5000; // Thời gian sáng LED vàng 1 
int time_green_1 = 15000; // Thời gian sáng LED xanh 1 
int time_red_2 = 30000;  // Thời gian sáng LED đỏ 2 
int time_yellow_2 = 5000; // Thời gian sáng LED vàng 2 
int time_green_2 = 15000; // Thời gian sáng LED xanh 2 

// Biến trạng thái và bước điều khiển
int mode = 0; // 0 = Manual (thủ công), 1 = Auto (tự động)
int step = 0; // Bước điều khiển đèn giao thông
unsigned long lastMillis = 0;

// Tạo Blynk Timer
BlynkTimer timer;

//  gửi thời gian hoạt động lên Blynk (V10)
void sendRuntimeToBlynk() {
  Blynk.virtualWrite(V10, millis() / 1000); // Thời gian tính bằng giây
}

// Hàm xử lý trạng thái đèn giao thông
void controlTrafficLights() {
  static unsigned long stepStartTime = 0;
  unsigned long currentMillis = millis();

  // Kiểm tra thời gian qua mỗi bước (sử dụng thời gian từng đèn)
  if (currentMillis - stepStartTime >= 2000) {
    stepStartTime = currentMillis;

    switch (step) {
      case 0: // Đèn đỏ 1 sáng, đèn xanh 2 sáng
        digitalWrite(red_LED1, HIGH);
        digitalWrite(yellow_LED1, LOW);
        digitalWrite(green_LED1, LOW);
        digitalWrite(red_LED2, LOW);
        digitalWrite(yellow_LED2, LOW);
        digitalWrite(green_LED2, HIGH);
        
        step = 1;
        break;

      case 1: // Đèn đỏ 1 sáng, đèn vàng 2 sáng
        digitalWrite(red_LED1, HIGH);
        digitalWrite(yellow_LED1, LOW);
        digitalWrite(green_LED1, LOW);
        digitalWrite(red_LED2, LOW);
        digitalWrite(yellow_LED2, HIGH);
        digitalWrite(green_LED2, LOW);
        
        step = 2;
        break;

      case 2: // Đèn xanh 1 sáng, đèn đỏ 2 sáng
        digitalWrite(red_LED1, LOW);
        digitalWrite(yellow_LED1, LOW);
        digitalWrite(green_LED1, HIGH);
        digitalWrite(red_LED2, HIGH);
        digitalWrite(yellow_LED2, LOW);
        digitalWrite(green_LED2, LOW);
       
        step = 3;
        break;

      case 3: // Đèn vàng 1 sáng, đèn đỏ 2 sáng
        digitalWrite(red_LED1, LOW);
        digitalWrite(yellow_LED1, HIGH);
        digitalWrite(green_LED1, LOW);
        digitalWrite(red_LED2, HIGH);
        digitalWrite(yellow_LED2, LOW);
        digitalWrite(green_LED2, LOW);
       
        step = 0; // Reset về bước đầu tiên
        break;
    }
  }
}

//  chế độ thủ công (Manual Mode) từ Button Widgets
BLYNK_WRITE(V4) { digitalWrite(red_LED1, param.asInt()); }
BLYNK_WRITE(V1) { digitalWrite(yellow_LED1, param.asInt()); }
BLYNK_WRITE(V2) { digitalWrite(green_LED1, param.asInt()); }
BLYNK_WRITE(V5) { digitalWrite(red_LED2, param.asInt()); }
BLYNK_WRITE(V6) { digitalWrite(yellow_LED2, param.asInt()); }
BLYNK_WRITE(V7) { digitalWrite(green_LED2, param.asInt()); }

// Chuyển chế độ qua Menu Widget (V8)
BLYNK_WRITE(V8) {
  mode = param.asInt(); // 0 = Manual, 1 = Auto
}

// Cập nhật thời gian sáng của LED đỏ 1
BLYNK_WRITE(V11) {
  time_red_1 = param.asInt() * 1000; //  ms
}

// Cập nhật thời gian sáng của LED vàng 1
BLYNK_WRITE(V12) {
  time_yellow_1 = param.asInt() * 1000; //  ms
}

// Cập nhật thời gian sáng của LED xanh 1
BLYNK_WRITE(V13) {
  time_green_1 = param.asInt() * 1000; // ms
}

// Cập nhật thời gian sáng của LED đỏ 2
BLYNK_WRITE(V14) {
  time_red_2 = param.asInt() * 1000; // ms
}

// Cập nhật thời gian sáng của LED vàng 2
BLYNK_WRITE(V15) {
  time_yellow_2 = param.asInt() * 1000; //  ms
}

// Cập nhật thời gian sáng của LED xanh 2
BLYNK_WRITE(V16) {
  time_green_2 = param.asInt() * 1000; //  ms
}

void setup() {
  // giao tiếp Serial với tốc độ truyền là 115200baud 
  Serial.begin(115200);

  // GPIO liên kết LED đầu ra
  pinMode(red_LED1, OUTPUT);
  pinMode(yellow_LED1, OUTPUT);
  pinMode(green_LED1, OUTPUT);
  pinMode(red_LED2, OUTPUT);
  pinMode(yellow_LED2, OUTPUT);
  pinMode(green_LED2, OUTPUT);

  // Kết nối với Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Gửi thời gian chạy đến Blynk mỗi giây
  timer.setInterval(1000L, sendRuntimeToBlynk);
}
//duy trì kết nối esp32 với blynk
void loop() {
  Blynk.run();
  timer.run();

  if (mode == 1) { // Chế độ tự động
    if (millis() - lastMillis > 2000) { // Điều khiển mỗi 2 giây
      lastMillis = millis();
      controlTrafficLights();
    }
  }
}
