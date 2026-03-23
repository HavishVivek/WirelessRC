#ifndef WirelessRC_h
#define WirelessRC_h

#define WIRELESSRC_OLED  // add this line

#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

// ─────────────────────────────────────────────
//  Optional OLED support
//  To enable: #define WIRELESSRC_OLED before including WirelessRC.h
//  Requires: Adafruit SSD1306 + Adafruit GFX libraries
// ─────────────────────────────────────────────
#ifdef WIRELESSRC_OLED
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
#endif

// ─────────────────────────────────────────────
//  WirelessRCCar  –  Receiver / Car side
// ─────────────────────────────────────────────
class WirelessRCCar {
public:
  /**
   * @param in1  Motor A direction pin 1  (default 2)
   * @param in2  Motor A direction pin 2  (default 4)
   * @param ena  Motor A PWM enable pin   (default 3)
   * @param in3  Motor B direction pin 1  (default 5)
   * @param in4  Motor B direction pin 2  (default 7)
   * @param enb  Motor B PWM enable pin   (default 6)
   * @param motorSpeed  PWM speed 0-255   (default 255)
   * @param deadzoneL   Lower deadzone    (default 553)
   * @param deadzoneH   Upper deadzone    (default 753)
   */
  WirelessRCCar(uint8_t in1 = 2,  uint8_t in2 = 4,  uint8_t ena = 3,
                uint8_t in3 = 5,  uint8_t in4 = 7,  uint8_t enb = 6,
                int motorSpeed = 255,
                int deadzoneL  = 553, int deadzoneH = 753);

  /** Call once in setup(). Returns false if RF driver fails to init. */
  bool begin();

  /**
   * Call every loop(). Checks for a new RF packet and drives motors.
   * Returns true when a valid packet was received this cycle.
   */
  bool update();

  // Motor primitives – available for manual control outside of update()
  void setMotors(bool m1fwd, bool m2fwd, int speed);
  void stopMotors();

  // Last received values (valid after update() returns true)
  int  lastX()      const { return _lastX; }
  int  lastY()      const { return _lastY; }
  bool lastButton() const { return _lastBtn; }

private:
  RH_ASK _driver;

  uint8_t _in1, _in2, _ena;
  uint8_t _in3, _in4, _enb;
  int     _motorSpeed;
  int     _deadzoneL, _deadzoneH;

  int  _lastX   = 512;
  int  _lastY   = 512;
  bool _lastBtn = true;   // HIGH = not pressed (INPUT_PULLUP)
  bool _autoDone = false;

  void _handleAutoRoutine();
  void _handleJoystick();
};


// ─────────────────────────────────────────────
//  WirelessRCController  –  Transmitter side
// ─────────────────────────────────────────────
class WirelessRCController {
public:
  /**
   * @param joystickX  Analog pin for X axis  (default A1)
   * @param joystickY  Analog pin for Y axis  (default A0)
   * @param buttonPin  Digital pin for button (default 2, uses INPUT_PULLUP)
   * @param txDelay    Milliseconds between transmissions (default 100)
   */
  WirelessRCController(uint8_t joystickX = A1, uint8_t joystickY = A0,
                       uint8_t buttonPin = 2,  uint16_t txDelay = 100);

  /** Call once in setup(). Returns false if RF driver fails to init. */
  bool begin();

  /**
   * Call every loop(). Reads joystick + button and transmits if interval elapsed.
   * Returns true when a packet was sent this cycle.
   */
  bool update();

  // Last transmitted values
  int  lastX()      const { return _lastX; }
  int  lastY()      const { return _lastY; }
  bool lastButton() const { return _lastBtn; }

private:
  RH_ASK   _driver;
  uint8_t  _joyX, _joyY, _btnPin;
  uint16_t _txDelay;
  uint32_t _lastSend = 0;

  int  _lastX   = 512;
  int  _lastY   = 512;
  bool _lastBtn = true;
};

// ─────────────────────────────────────────────
//  WirelessRCControllerOLED  –  Controller with SSD1306 display
//  Only compiled when #define WIRELESSRC_OLED is set
// ─────────────────────────────────────────────
#ifdef WIRELESSRC_OLED

#define WIRELESSRC_OLED_WIDTH  128
#define WIRELESSRC_OLED_HEIGHT  32
#define WIRELESSRC_OLED_RESET   -1

class WirelessRCControllerOLED : public WirelessRCController {
public:
  /**
   * @param i2cAddr   I2C address of OLED (default 0x3C)
   * @param joystickX Analog pin for X axis  (default A1)
   * @param joystickY Analog pin for Y axis  (default A0)
   * @param buttonPin Digital pin for button (default 2)
   * @param txDelay   Ms between transmissions (default 100)
   */
  WirelessRCControllerOLED(uint8_t i2cAddr  = 0x3C,
                            uint8_t joystickX = A1,
                            uint8_t joystickY = A0,
                            uint8_t buttonPin = 2,
                            uint16_t txDelay  = 100);

  /** Call once in setup(). Initialises OLED + RF driver. */
  bool begin();

  /**
   * Call every loop(). Transmits joystick packet and refreshes OLED.
   * Returns true when a packet was sent this cycle.
   */
  bool update();

  /** Toggle auto mode on/off */
  void setAutoMode(bool enabled) { _autoMode = enabled; }
  bool autoMode() const          { return _autoMode; }

private:
  Adafruit_SSD1306 _display;
  uint8_t          _i2cAddr;
  unsigned long    _lastSentMs = 0;
  bool             _autoMode   = false;

  void _updateDisplay();
  void _startupAnimation();
};

#endif // WIRELESSRC_OLED

#endif // WirelessRC_h
