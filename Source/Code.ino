#include <Servo.h>
#include <LiquidCrystal_SoftI2C.h>
#include <SoftwareWire.h>

SoftwareWire myWire(8, 9); 
LiquidCrystal_I2C lcd(0x27, 16, 2, &myWire);

Servo base;  // Servo orizontal
Servo top;   // Servo vertical
const int threshold = 10;

// PINI pentru fotorezistori
const int pinTL = A2;
const int pinDL = A3;
const int pinTR = A4;
const int pinDR = A5;

// Valori pentru ACS712
const int acsPin = A0;
int sensitivity = 185;       // 185 mV/A pentru modulul de 5A
int offsetvoltage = 2500;    // offset în mV
int adcvalue = 0;
double voltage = 0;
double current = 0;

// Timere
unsigned long lastSensorRead = 0;
unsigned long lastLDRRead = 0;
const unsigned long sensorInterval = 500;  // 500ms
const unsigned long ldrInterval = 100;     // 100ms

// Media a 10 citiri consecutive ADC
int smoothAnalogRead(int pin) {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    delay(2);
  }
  return map(sum / 10, 0, 1023, 0, 180);
}

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("POWER ON");
  
  base.attach(2);  // Digital PIN 2 pentru servo orizontal
  top.attach(3);   // Digital PIN 3 pentru servo vertical
  Serial.begin(9600);

  base.write(90);  // Default
  top.write(45);  // Default
  delay(1500);
  lcd.clear();
}

void loop() {
  // === Citire ACS712 la 500ms ===
  if (millis() - lastSensorRead >= sensorInterval) {
    lastSensorRead = millis();

    adcvalue = analogRead(acsPin);
    voltage = (adcvalue / 1024.0) * 5000.0;
    current = (voltage - offsetvoltage) / sensitivity;

    lcd.setCursor(0, 0);
    lcd.print("V:");
    lcd.print(voltage, 0);
    lcd.print("mV ");

    lcd.setCursor(0, 1);
    lcd.print("I:");
    lcd.print(current, 2);
    lcd.print("A   ");

    Serial.print("Tensiune senzor: ");
    Serial.print(voltage, 1);
    Serial.print(" mV | Curent: ");
    Serial.print(current, 3);
    Serial.println(" A");
  }

  // === Citire fotorezistori la 100ms ===
  if (millis() - lastLDRRead >= ldrInterval) {
    lastLDRRead = millis();

    // Citirile senzorilor
    int tl = smoothAnalogRead(pinTL);
    int dl = 60 + smoothAnalogRead(pinDL);
    int tr = smoothAnalogRead(pinTR);
    int dr = 20 + smoothAnalogRead(pinDR);
  
    // Media top, bottom, left si right
    int avgT = (tl + tr) / 2;
    int avgB = (dl + dr) / 2;
    int avgL = (tl + dl) / 2;
    int avgR = (tr + dr) / 2;
  
    Serial.print("TL: "); Serial.print(tl);
    Serial.print(" | TR: "); Serial.print(tr);
    Serial.print(" | DL: "); Serial.print(dl);
    Serial.print(" | DR: "); Serial.println(dr);

    // Miscare doar daca diferenta este semnificativa
    if (abs(avgT - avgB) > threshold) {
      UpDown(avgT, avgB);
    }
    if (abs(avgL - avgR) > threshold) {
      LeftRight(avgL, avgR);
    }
  }
}

void UpDown(int avgT, int avgB) {
  int pos = top.read();
  if (avgT > avgB) {
    pos++;
    Serial.println("Coboara");
  } else {
    pos--;
    Serial.println("Urca");
  }
  pos = constrain(pos, 0, 170);
  top.write(pos);
}

void LeftRight(int avgL, int avgR) {
  int pos = base.read();
  if (avgL > avgR) {
    pos--;  // Move right
    Serial.println("Stanga");
  } else {
    pos++;  // Move left
    Serial.println("Dreapta");
  }
  pos = constrain(pos, 0, 180);
  base.write(pos);
}

