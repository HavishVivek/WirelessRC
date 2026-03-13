# WirelessRC

A simple Arduino library for building wireless RC cars using the **RadioHead ASK** RF module.  
It splits the project into two clean classes:

| Class | Role | Upload to |
|---|---|---|
| `WirelessRCCar` | Receives joystick packets and drives motors | Car Arduino |
| `WirelessRCController` | Reads joystick + button and transmits packets | Controller Arduino |

---

## Dependencies

- [RadioHead](https://www.airspayce.com/mikem/arduino/RadioHead/) — install via the Arduino Library Manager

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

---

## Custom Pin / Speed Configuration

```cpp
// WirelessRCCar(in1, in2, ena, in3, in4, enb, speed, deadzoneL, deadzoneH)
WirelessRCCar car(2, 4, 3, 5, 7, 6, 200, 400, 624);

// WirelessRCController(joyX, joyY, buttonPin, txDelayMs)
WirelessRCController ctrl(A1, A0, 2, 50);
```

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

---

## License

MIT
