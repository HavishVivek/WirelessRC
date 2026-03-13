/*
  ControllerTransmitter.ino – WirelessRC library example
  --------------------------------------------------------
  Upload this to the Arduino acting as the handheld controller.
  Uses default pins:
    Joystick X → A1
    Joystick Y → A0
    Button     → D2  (INPUT_PULLUP – press = LOW)
  RF transmitter data pin → pin 12 (RadioHead ASK default)
*/

#include <WirelessRC.h>

// To customise: WirelessRCController ctrl(joyX, joyY, buttonPin, txDelayMs);
WirelessRCController ctrl;

void setup() {
  Serial.begin(9600);

  if (!ctrl.begin()) {
    Serial.println("RF driver init failed – check wiring.");
    while (true);
  }

  Serial.println("WirelessRC Controller ready.");
}

void loop() {
  if (ctrl.update()) {
    Serial.print("Sent → X: ");  Serial.print(ctrl.lastX());
    Serial.print(" Y: ");         Serial.print(ctrl.lastY());
    Serial.print(" Btn: ");       Serial.println(ctrl.lastButton());
  }
}
