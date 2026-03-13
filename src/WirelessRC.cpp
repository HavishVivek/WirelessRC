#include "WirelessRC.h"

// ══════════════════════════════════════════════
//  WirelessRCCar
// ══════════════════════════════════════════════

WirelessRCCar::WirelessRCCar(uint8_t in1, uint8_t in2, uint8_t ena,
                              uint8_t in3, uint8_t in4, uint8_t enb,
                              int motorSpeed, int deadzoneL, int deadzoneH)
  : _in1(in1), _in2(in2), _ena(ena),
    _in3(in3), _in4(in4), _enb(enb),
    _motorSpeed(motorSpeed),
    _deadzoneL(deadzoneL), _deadzoneH(deadzoneH)
{}

bool WirelessRCCar::begin() {
  pinMode(_in1, OUTPUT); pinMode(_in2, OUTPUT); pinMode(_ena, OUTPUT);
  pinMode(_in3, OUTPUT); pinMode(_in4, OUTPUT); pinMode(_enb, OUTPUT);
  stopMotors();
  return _driver.init();
}

bool WirelessRCCar::update() {
  uint8_t buf[30];
  uint8_t buflen = sizeof(buf);

  if (!_driver.recv(buf, &buflen)) return false;

  buf[buflen] = '\0';
  sscanf((char*)buf, "%d,%d,%d", &_lastX, &_lastY, (int*)&_lastBtn);

  _handleAutoRoutine();
  _handleJoystick();
  return true;
}

void WirelessRCCar::setMotors(bool m1fwd, bool m2fwd, int speed) {
  digitalWrite(_in1, m1fwd ? HIGH : LOW);
  digitalWrite(_in2, m1fwd ? LOW  : HIGH);
  digitalWrite(_in3, m2fwd ? HIGH : LOW);
  digitalWrite(_in4, m2fwd ? LOW  : HIGH);
  analogWrite(_ena, speed);
  analogWrite(_enb, speed);
}

void WirelessRCCar::stopMotors() {
  digitalWrite(_in1, LOW); digitalWrite(_in2, LOW);
  digitalWrite(_in3, LOW); digitalWrite(_in4, LOW);
  analogWrite(_ena, 0);    analogWrite(_enb, 0);
}

// ── private helpers ───────────────────────────

void WirelessRCCar::_handleAutoRoutine() {
  if (_lastBtn == LOW && !_autoDone) {
    setMotors(true,  true,  _motorSpeed); delay(5000);
    stopMotors();                         delay(500);
    setMotors(false, false, _motorSpeed); delay(5000);
    stopMotors();
    _autoDone = true;
  }
  if (_lastBtn == HIGH) _autoDone = false;
}

void WirelessRCCar::_handleJoystick() {
  if (_lastY < _deadzoneL) {
    setMotors(true, true, _motorSpeed);     // Forward
  } else if (_lastY > _deadzoneH) {
    setMotors(false, false, _motorSpeed);   // Backward
  } else {
    stopMotors();                           // Dead zone
  }
}


// ══════════════════════════════════════════════
//  WirelessRCController
// ══════════════════════════════════════════════

WirelessRCController::WirelessRCController(uint8_t joystickX, uint8_t joystickY,
                                           uint8_t buttonPin, uint16_t txDelay)
  : _joyX(joystickX), _joyY(joystickY),
    _btnPin(buttonPin), _txDelay(txDelay)
{}

bool WirelessRCController::begin() {
  pinMode(_btnPin, INPUT_PULLUP);
  return _driver.init();
}

bool WirelessRCController::update() {
  uint32_t now = millis();
  if (now - _lastSend < _txDelay) return false;
  _lastSend = now;

  _lastX   = analogRead(_joyX);
  _lastY   = analogRead(_joyY);
  _lastBtn = digitalRead(_btnPin);

  char message[30];
  sprintf(message, "%d,%d,%d", _lastX, _lastY, (int)_lastBtn);

  _driver.send((uint8_t*)message, strlen(message));
  _driver.waitPacketSent();
  return true;
}
