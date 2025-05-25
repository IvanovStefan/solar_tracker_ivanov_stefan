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
  lcd.print("Hello, World!");

  
  base.attach(2);  // Digital PIN 2 pentru servo orizontal
  top.attach(3);   // Digital PIN 3 pentru servo vertical
  Serial.begin(9600);

  base.write(90);  // Default
  top.write(45);  // Default
}

void loop() {
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

  delay(100);
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

