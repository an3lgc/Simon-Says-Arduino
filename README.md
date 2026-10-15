
# Simon Says — Arduino Memory Game

An interactive Simon Says memory game built on an Arduino Uno, combining digital I/O, I2C communication, and frequency modulation to turn code into physical lights, sound, and on-screen text.

## How It Works

The game flashes a growing sequence of LEDs (with a matching tone for each), and the player must press the corresponding buttons in the same order. Get it right, and the sequence grows by one. Get it wrong, and the game resets.

## Features

- Scrolling welcome message and live score display on a 16x2 I2C LCD
- 4-LED, 4-button gameplay with unique tones per button
- Piezo buzzer audio using `tone()` / `noTone()` (no external libraries needed for sound)
- **Yes** button to start a game
- **No** button doubles as **Retry** (at the welcome screen) and **Exit** (mid-game, backs out to the welcome screen)

## Hardware

| Component | Arduino Pin |
|---|---|
| LED 1 | 2 |
| LED 2 | 3 |
| LED 3 | 4 |
| LED 4 | 5 |
| Gameplay Button 1 | 8 |
| Gameplay Button 2 | 9 |
| Gameplay Button 3 | 10 |
| Gameplay Button 4 | 11 |
| Buzzer (piezo) | 12 |
| Yes Button | 13 |
| No / Retry / Exit Button | 0 |
| 16x2 I2C LCD | SDA → A4, SCL → A5 |

Each LED uses a 330Ω resistor in series. Buttons use the Arduino's internal pull-up resistors (`INPUT_PULLUP`), so no external pull-up/pull-down resistors are needed — just wire each button between its pin and GND.

## Libraries Required

- [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C) — for the LCD display

No external library is needed for the buzzer; this project uses the Arduino's built-in `tone()` / `noTone()` functions.

## Setup

1. Wire the circuit according to the pin table above.
2. Install the `LiquidCrystal_I2C` library (Library Manager, or as a .zip if needed).
3. Upload `Simon_Says.ino` to your Arduino Uno.
4. Open the Serial Monitor (9600 baud) if you want to watch button states and round scores while debugging.

## What I Learned

**`tone()` and frequency manipulation** — Arduino pins can only output HIGH or LOW, so there's no native way to produce a smooth analog sound wave. The `tone()` function works around this by rapidly switching a pin on and off at a set rate, generating a square wave. Changing that switching rate (frequency, in Hz) changes the pitch you hear — which is how a single piezo buzzer on one pin can play an entire range of musical notes just by varying timing in software.

**LCD displays and registers** — The 16x2 LCD stores what it shows using two internal registers: a **Command** register (handles instructions like clearing the screen or moving the cursor) and a **Data** register (holds the actual characters being displayed). Understanding this distinction made it clear why functions like `lcd.clear()` and `lcd.print()` behave so differently under the hood, even though they're both one-line calls in code.

**I2C communication** — Normally, an LCD needs 6+ wires connected directly to the Arduino just to function. The I2C backpack on the LCD compresses all of that down to two wires (SDA and SCL), using a shared communication protocol instead of a dedicated wire per signal. This freed up several pins for buttons and LEDs, and also introduced the idea of device addressing (`0x27` in this project) — a concept that scales to connecting multiple I2C devices on the same two wires.

## Credits

Originally based on a Simon Says LCD project by Vineeth Balachandran, modified for:
- Piezo buzzer support via `tone()`/`noTone()` instead of the Tone library
- Reliable button input via `INPUT_PULLUP`
- Pin-agnostic sequence/input logic (array-position based instead of hardcoded pin numbers)
- Retry and mid-game Exit functionality on the No button
