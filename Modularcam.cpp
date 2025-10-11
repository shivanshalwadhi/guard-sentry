#include <Wire.h>

#define VSYNC_PIN 8
#define HREF_PIN  9
#define PCLK_PIN  10
#define XCLK_PIN  11
#define D0_PIN    2
#define D1_PIN    3
#define D2_PIN    4
#define D3_PIN    5
#define D4_PIN    6
#define D5_PIN    7
#define D6_PIN    12
#define D7_PIN    13
#define RESET_PIN A0  // If controlling RESET from Arduino

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(VSYNC_PIN, INPUT);
  pinMode(HREF_PIN, INPUT);
  pinMode(PCLK_PIN, INPUT);
  pinMode(XCLK_PIN, OUTPUT);
  pinMode(D0_PIN, INPUT);
  pinMode(D1_PIN, INPUT);
  pinMode(D2_PIN, INPUT);
  pinMode(D3_PIN, INPUT);
  pinMode(D4_PIN, INPUT);
  pinMode(D5_PIN, INPUT);
  pinMode(D6_PIN, INPUT);
  pinMode(D7_PIN, INPUT);
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);

  setupXCLK();

  writeCamRegister(0x12, 0x80); // Camera reset
  delay(200);

  Serial.println("OV7670 Ready. Send trigger for capture...");
}

void loop() {
  if (Serial.available()) {
    Serial.read(); // any signal to start

    Serial.println("CAPTURE_BEGIN");
    while (digitalRead(VSYNC_PIN) == HIGH);
    while (digitalRead(VSYNC_PIN) == LOW);

    for (int row = 0; row < 480; row++) {
      while (digitalRead(HREF_PIN) == LOW);
      while (digitalRead(HREF_PIN) == HIGH) {
        while (digitalRead(PCLK_PIN) == LOW);
        byte pixel =
          (digitalRead(D0_PIN) << 0) |
          (digitalRead(D1_PIN) << 1) |
          (digitalRead(D2_PIN) << 2) |
          (digitalRead(D3_PIN) << 3) |
          (digitalRead(D4_PIN) << 4) |
          (digitalRead(D5_PIN) << 5) |
          (digitalRead(D6_PIN) << 6) |
          (digitalRead(D7_PIN) << 7);
        Serial.write(pixel);
        while (digitalRead(PCLK_PIN) == HIGH);
      }
    }
    Serial.println("CAPTURE_END");
  }
}

void setupXCLK() {
  pinMode(XCLK_PIN, OUTPUT);
  TCCR2A = _BV(COM2A0) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS20);
  OCR2A = 0;
}

void writeCamRegister(byte reg, byte val) {
  Wire.beginTransmission(0x42 >> 1);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}
