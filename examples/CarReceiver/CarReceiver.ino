/*
  CarReceiver.ino – WirelessRC library example
  ------------------------------------------------
  Upload this to the Arduino mounted on the RC car.
  Uses default pins:
    Motor A → IN1=2, IN2=4, ENA=3
    Motor B → IN3=5, IN4=7, ENB=6
  RF receiver data pin → pin 11 (RadioHead ASK default)
*/

#include <WirelessRC.h>

// All pins and settings match the original project defaults.
// To customise, pass arguments: WirelessRCCar car(in1, in2, ena, in3, in4, enb, speed, dzLow, dzHigh);
WirelessRCCar car;

void setup() {
  Serial.begin(115200);

  if (!car.begin()) {
    Serial.println("RF driver init failed – check wiring.");
    while (true);
  }

  Serial.println("WirelessRC Car ready.");
}

void loop() {
  if (car.update()) {
    Serial.print("X: ");  Serial.print(car.lastX());
    Serial.print("\tY: "); Serial.print(car.lastY());
    Serial.print("\tBtn: "); Serial.println(car.lastButton());
  }
}
