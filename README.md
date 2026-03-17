# WirelessRC

A simple Arduino library for building wireless RC cars using the **RadioHead ASK** RF module.  
It splits the project into clean classes:

| Class | Role | Upload to |
|---|---|---|
| `WirelessRCCar` | Receives joystick packets and drives motors | Car Arduino |
| `WirelessRCController` | Reads joystick + button and transmits packets | Controller Arduino |
| `WirelessRCControllerOLED` | Controller with built-in SSD1306 OLED display | Controller Arduino |

---

## Dependencies

- [RadioHead](https://www.airspayce.com/mikem/arduino/RadioHead/) — install via the Arduino Library Manager

**Optional (only needed if using `WirelessRCControllerOLED`):**
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) — install via the Arduino Library Manager
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) — install via the Arduino Library Manager

---

## Wiring (defaults)

### Car (receiver)
| Signal | Pin |
|---|---|
| Motor A IN1 | 2 |
| Motor A IN2 | 4 |
| Motor A ENA (PWM) | 3 |
| Motor B IN3 | 5 |
| Motor B IN4 | 7 |
| Motor B ENB (PWM) | 6 |
| RF data (RH_ASK) | 11 |

### Controller (transmitter)
| Signal | Pin |
|---|---|
| Joystick X | A1 |
| Joystick Y | A0 |
| Button | 2 |
| RF data (RH_ASK) | 12 |

### Controller with OLED (additional wiring)
| Signal | Pin |
|---|---|
| OLED VCC | 5V |
| OLED GND | GND |
| OLED SDA | A4 |
| OLED SCL | A5 |

---

## Quick Start

**Car sketch:**
```cpp
#include <WirelessRC.h>

WirelessRCCar car;   // uses all defaults

void setup() { car.begin(); }
void loop()  { car.update(); }
```

**Controller sketch:**
```cpp
#include <WirelessRC.h>

WirelessRCController ctrl;  // uses all defaults

void setup() { ctrl.begin(); }
void loop()  { ctrl.update(); }
```

**Controller with OLED sketch:**
```cpp
#define WIRELESSRC_OLED   // must be before the include
#include <WirelessRC.h>

WirelessRCControllerOLED ctrl;  // uses all defaults

void setup() { ctrl.begin(); }
void loop()  { ctrl.update(); }
```

---

## Custom Pin / Speed Configuration

```cpp
// WirelessRCCar(in1, in2, ena, in3, in4, enb, speed, deadzoneL, deadzoneH)
WirelessRCCar car(2, 4, 3, 5, 7, 6, 200, 400, 624);

// WirelessRCController(joyX, joyY, buttonPin, txDelayMs)
WirelessRCController ctrl(A1, A0, 2, 50);

// WirelessRCControllerOLED(i2cAddr, joyX, joyY, buttonPin, txDelayMs)
WirelessRCControllerOLED ctrl(0x3C, A1, A0, 2, 50);
```

---

## OLED Display

When using `WirelessRCControllerOLED`, the SSD1306 128x32 display shows three rows of live data:

```
MODE: MANUAL [OK]
X:512  Y:498
BTN: released
```

| Element | Description |
|---|---|
| `MODE: MANUAL` | Joystick is controlling the car |
| `MODE: AUTO` | Auto-drive routine is active |
| `[OK]` | Transmitting successfully |
| `[--]` | No transmission in over 1 second |
| `X / Y` | Raw joystick values (0–1023, center ~512) |
| `BTN` | Button state — PRESSED or released |

**Startup animation:** on power-on, "WirelessRC" types out letter by letter followed by "Controller Ready".

**Auto mode:** pressing the joystick button toggles between manual and auto mode. The current mode is always visible on the OLED.

> **Note:** `#define WIRELESSRC_OLED` must appear **before** `#include <WirelessRC.h>` in your sketch. Without it the OLED class is not compiled and the Adafruit libraries are not required.

---

## API Reference

### `WirelessRCCar`
| Method | Description |
|---|---|
| `begin()` | Initialises motor pins and RF driver. Returns `false` on RF failure. |
| `update()` | Checks for incoming packet and drives motors. Returns `true` when packet received. |
| `setMotors(m1fwd, m2fwd, speed)` | Manually drive both motors. |
| `stopMotors()` | Immediately stop both motors. |
| `lastX()` / `lastY()` | Last received joystick values (0–1023). |
| `lastButton()` | Last received button state (`LOW` = pressed). |

### `WirelessRCController`
| Method | Description |
|---|---|
| `begin()` | Initialises button pin and RF driver. Returns `false` on RF failure. |
| `update()` | Reads joystick + button and transmits if interval elapsed. Returns `true` when sent. |
| `lastX()` / `lastY()` | Last transmitted joystick values. |
| `lastButton()` | Last transmitted button state. |

### `WirelessRCControllerOLED` *(requires `#define WIRELESSRC_OLED`)*
Inherits all methods from `WirelessRCController` plus:

| Method | Description |
|---|---|
| `begin()` | Initialises OLED, plays startup animation, then inits RF driver. |
| `update()` | Transmits packet and refreshes OLED every loop. |
| `setAutoMode(bool)` | Manually set auto mode on or off. |
| `autoMode()` | Returns current auto mode state. |

---

## License

MIT
