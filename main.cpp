#include <Arduino.h>
#define BLYNK_TEMPLATE_ID "TMPL6OirvDI4E"
#define BLYNK_TEMPLATE_NAME "traffic light"
#define BLYNK_AUTH_TOKEN "6RcSO1QPMu6cr_nO9gjMF9sjZcaUwzyX"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


#define BLYNK_PRINT Serial

// Thông tin WiFi
char ssid[] = "Lam Giang";
char pass[] = "0372363897";

// Định nghĩa các chân GPIO cho đèn LED
const int red_LED1 = 14;      // Đèn đỏ 1
const int yellow_LED1 = 12;   // Đèn vàng 1
const int green_LED1 = 13;    // Đèn xanh 1
const int red_LED2 = 25;      // Đèn đỏ 2
const int yellow_LED2 = 26;   // Đèn vàng 2
const int green_LED2 = 27;    // Đèn xanh 2

// Biến trạng thái và thời gian
int mode = 0; // 0 = Manual (thủ công), 1 = Auto (tự động)
int step = 0; // Bước điều khiển đèn giao thông
unsigned long lastMillis = 0;

// Tạo Blynk Timer
BlynkTimer timer;

// Hàm gửi thời gian hoạt động của hệ thống đến Blynk (V2)
void sendRuntimeToBlynk() {
  Blynk.virtualWrite(V2, millis() / 1000); // Thời gian tính bằng giây
}

// Hàm xử lý trạng thái đèn giao thông
void controlTrafficLights() {
  switch (step) {
    case 0: // Đèn đỏ 1, đèn xanh 2
      digitalWrite(red_LED1, HIGH);
      digitalWrite(yellow_LED1, LOW);
      digitalWrite(green_LED1, LOW);
      digitalWrite(red_LED2, LOW);
      digitalWrite(yellow_LED2, LOW);
      digitalWrite(green_LED2, HIGH);
      break;
    case 1: // Đèn đỏ 1, đèn vàng 2
      digitalWrite(red_LED1, HIGH);
      digitalWrite(yellow_LED1, LOW);
      digitalWrite(green_LED1, LOW);
      digitalWrite(red_LED2, LOW);
      digitalWrite(yellow_LED2, HIGH);
      digitalWrite(green_LED2, LOW);
      break;
    case 2: // Đèn đỏ 1, đèn đỏ 2
      digitalWrite(red_LED1, HIGH);
      digitalWrite(yellow_LED1, LOW);
      digitalWrite(green_LED1, LOW);
      digitalWrite(red_LED2, HIGH);
      digitalWrite(yellow_LED2, LOW);
      digitalWrite(green_LED2, LOW);
      break;
    case 3: // Đèn xanh 1, đèn đỏ 2
      digitalWrite(red_LED1, LOW);
      digitalWrite(yellow_LED1, LOW);
      digitalWrite(green_LED1, HIGH);
      digitalWrite(red_LED2, HIGH);
      digitalWrite(yellow_LED2, LOW);
      digitalWrite(green_LED2, LOW);
      break;
    case 4: // Đèn vàng 1, đèn đỏ 2
      digitalWrite(red_LED1, LOW);
      digitalWrite(yellow_LED1, HIGH);
      digitalWrite(green_LED1, LOW);
      digitalWrite(red_LED2, HIGH);
      digitalWrite(yellow_LED2, LOW);
      digitalWrite(green_LED2, LOW);
      step = -1; // Reset về bước đầu tiên
      break;
  }
  step++;
}

// Hàm xử lý chế độ thủ công (Manual Mode) từ Button Widgets
BLYNK_WRITE(V0) { digitalWrite(red_LED1, param.asInt()); }
BLYNK_WRITE(V1) { digitalWrite(yellow_LED1, param.asInt()); }
BLYNK_WRITE(V2) { digitalWrite(green_LED1, param.asInt()); }
BLYNK_WRITE(V3) { digitalWrite(red_LED2, param.asInt()); }
BLYNK_WRITE(V4) { digitalWrite(yellow_LED2, param.asInt()); }
BLYNK_WRITE(V5) { digitalWrite(green_LED2, param.asInt()); }

// Chuyển chế độ qua Menu Widget (V6)
BLYNK_WRITE(V6) {
  mode = param.asInt(); // 0 = Manual, 1 = Auto
}

void setup() {
  // Bật Serial để debug
  Serial.begin(115200);

  // Cấu hình các chân GPIO của đèn LED
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
