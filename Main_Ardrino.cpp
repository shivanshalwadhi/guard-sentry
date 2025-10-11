#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIR_PIN      2
#define SOUND_PIN    3
#define IR_PIN       5
#define LDR_PIN      A0
#define ARM_PIN      4
#define BUZZER_PIN   6
#define LED_PIN      7

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool armed = false;
bool lastButtonState = LOW;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);
  pinMode(IR_PIN, INPUT);
  pinMode(ARM_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  lcd.begin(16,2);
  lcd.backlight();
  Serial.begin(9600);
  lcd.setCursor(0,0); lcd.print("System Loading...");
  delay(1000);
}

void loop() {
  bool currentButtonState = digitalRead(ARM_PIN) == LOW;
  if (currentButtonState && !lastButtonState) {
    armed = !armed;
    delay(300);
  }
  lastButtonState = currentButtonState;

  int pirState = digitalRead(PIR_PIN);
  int soundState = digitalRead(SOUND_PIN);
  int irState = digitalRead(IR_PIN);
  int ldrValue = analogRead(LDR_PIN);

  int ldrThreshold = 400;
  int sensorsTriggered = 0;
  if (pirState == 1) sensorsTriggered++;
  if (soundState == 1) sensorsTriggered++;
  if (irState == 1) sensorsTriggered++;
  if (ldrValue < ldrThreshold) sensorsTriggered++;

  bool intruderAlert = armed && sensorsTriggered >= 2;

  digitalWrite(BUZZER_PIN, intruderAlert ? HIGH : LOW);
  digitalWrite(LED_PIN, intruderAlert ? HIGH : LOW);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(armed ? "State: ARMED  " : "State: UNARMED");
  lcd.setCursor(0,1);
  if (intruderAlert) {
    lcd.print("INTRUDER ALERT!");
  } else {
    lcd.print("P:");
    lcd.print(pirState);
    lcd.print(" S:");
    lcd.print(soundState);
    lcd.print(" I:");
    lcd.print(irState);
    lcd.print(" L:");
    lcd.print(ldrValue);
  }

  Serial.print(armed);
  Serial.print(",");
  Serial.print(intruderAlert);
  Serial.print(",");
  Serial.print(pirState);
  Serial.print(",");
  Serial.print(soundState);
  Serial.print(",");
  Serial.print(irState);
  Serial.print(",");
  Serial.println(ldrValue);

  delay(250);
}
