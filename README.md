# ⛵ RC Sailboat Autopilot

RC Sailboat Autopilot is an Arduino UNO Q-based autopilot TIPE project for an RC sailboat. It combines onboard sensors (magnetometer, wind sensor), rudder control, and a Python bridge for data logging. Feel free to read the MCOT (French), it explains some details about the whys of the project and includes some sources to understand the theory behind it.

## What this project does

- Reads boat state from multiple modules: communication input, magnetometer heading, rudder feedback, and apparent wind angle.
- Supports **manual mode** (rudder follows RC/command input) and **unmanned mode** (rudder command computed from a setpoint).
- Can steer using either:
  - **heading follow** (`heading_sp`), or
  - **apparent wind angle follow** (`awa_sp`).
- Logs experiment data to CSV via the Python app bridge.

## Hardware

- Runs on Arduino UNO Q (if you don't want to log data, a classic Arduino UNO R4 can make it through i guess).
- Uses the Adafruit PWM servo shield to control the rudder servo via I2C.
- Adafruit GPS Breakout for the GPS (not needed for the control, thus nice to plot paths after an experiment). --> not yet implemented 
- An HMC5883L magnetometer to measure the heading. (I know I should use an IMU to correct the distortion due to roll but I've struggled a lot with Adafruit BNO085 Breakout sensor)
- Custom 3D printed Windvane (a simple low friction potentiometer put on the top of the mast).
- A classic radio controller (with a switch) and receiver use to manually control the sail and the rest of the boat if needed.

## Repository layout

```text
.
├── app.yaml                 # App metadata for the runtime
├── python/
│   ├── main.py              # Providing a function that saves data via Arduino Bridge
|   └── DATA.py              # CSV logger
└── sketch/
    ├── sketch.ino           # Main Arduino control loop
    ├── CONFIG.h             # Tunable constants and feature flags
    ├── COM.*                # RC / communication input
    ├── COMPASS.*            # Heading source
    ├── WINDSENSOR.*         # Apparent wind angle source
    └── RUDDER.*             # Rudder actuation + feedback
```

## Control logic (high-level)

At each loop iteration:

1. Update all modules (`COM`, `COMPASS`, `WINDSENSOR`, `RUDDER`).
2. Save telemetry (`timestamp_ms, heading_deg, rudder_angle_deg, awa_deg, unmanned_status`).
3. Apply control:
   - if unmanned mode is active:
     - use heading control (`Kp * (heading_sp - heading)`), or
     - use AWA-follow control.
   - otherwise: pass through manual rudder command.

The loop period is configurable via `loop_period_ms` in `CONFIG.h` (default 100 ms).

## Configuration

Most parameters live in `sketch/CONFIG.h`.

Common values to adjust:

- Hardware pins (`PIN_*` constants)
- PWM/ADC calibration values
- `Kp` proportional gain
- `loop_period_ms`
- Feature flags:
  - `FULL_UNMANNED_MODE`
  - `AWA_FOLLOW_MODE`
- Setpoints:
  - `heading_sp`
  - `awa_sp`

## Data logging

Each experiment session produces two paired files in `<project>/logs`:

```text
session_000x.csv
session_000x.config.h
```

The `.csv` is pure tabular telemetry (loadable by any tool with no comment-line flags). The `.config.h` is a verbatim snapshot of `sketch/CONFIG.h` at session start, so each experiment keeps its configuration context alongside the data.

## Getting started

1. Open and review `sketch/CONFIG.h`.
2. Configure your board target (`fqbn`) in `sketch/sketch.yaml`.
3. Build and upload the sketch using your Arduino tooling.
4. Run the Python app runtime so `python/main.py` can receive bridge calls and log data.

## Notes

- `full_unmanned_mode = true` compiles a simplified loop focused on remote rudder commands.
- In normal mode (`false`), all sensors and logging are active.
- Start with conservative gain (`Kp`) and validate behavior in controlled conditions before open-water testing.


## License

No license file is currently provided in this repository.


## Useful links:

- Arduino Uno Q User Manual (useful to understand how the Arduino Uno Q works)
https://docs.arduino.cc/tutorials/uno-q/user-manual/
- Arduino App CLI (to understand how an Arduino Uno Q app runs)
https://github.com/arduino/arduino-app-cli
