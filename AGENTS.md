# AGENTS.md — ArduinoMid

Guidance for AI coding agents working in this repository.

## Project

Drop-in **MID** (Multi-Info Display) replacement for **Vauxhall/Opel Astra G & Frontera**, on **Arduino Mega 2560** (`atmega2560`, 16 MHz), using the original vehicle connector and a custom PCB.

- Firmware lives under `MID/` (open `MID/MID.ino`).
- Current UI: graphical **240×64** T6963 LCD via **U8g2**. The 16×2 path still exists but is deprecated.
- External docs: [wiki](https://github.com/fire1/ArduinoMid/wiki). Companion Android app: `eu.fire1.arduinomid`.
- License: MIT. Treat quality as beta; calibration is vehicle-specific.

## Layout

| Path | Purpose |
|------|---------|
| `MID/` | Only firmware source to edit for product behavior |
| `MID/lib/` | Domain modules (mostly **header-only** `.h`) |
| `MID/lib/core/` | AVR-free pure helpers (`MidMath`, `MidGear`, `MidLpgBits`) shared with host tests |
| `MID/lib/ui/` | LCD, menus, glyphs, fonts, graphics |
| `MID/lib/ui/language/` | One language pack per file |
| `libraries/` | Vendor library **zips** (extracted dirs are gitignored) |
| `tests/` | Hardware bring-up sketches (not unit/CI tests) |
| `unit_tests/` | Host Catch2 suite; run via CMake (`unit_tests/build`) |
| `resources/` | PCB, Gerber, icons, 3D holders |
| `cmake/` | Optional arduino-cmake toolchain (paths often stale) |
| `compiled/` | Prebuilt hex dumps |

## Config layers (do not mix)

1. **`MID/conf.h`** — per-vehicle / per-build overrides. Language, radio profile, LPG, DRL, vehicle personality, optional correction blocks. **Put custom settings here.**
2. **`MID/MID.h`** — platform defaults: pin map, `SCREEN`, baud, interrupt pins, includes `conf.h` then `glob.h`. Change pins only with hardware awareness.
3. **`MID/glob.h`** — shared types/interfaces (`SavedData`, `TripData`, UI interfaces). Changing structs affects EEPROM layout and serial inject — treat as ABI.

Language: enable **exactly one** `#define LANG_*` in `conf.h` (packs share `#ifndef ARDUINO_MID_LAN`).

## Architecture

```
MID.ino → MID.h ← conf.h → glob.h
       → InitObj.h (sensors/services)
       → InitLcd.h (display + menu for SCREEN)
```

Core objects (see `InitObj.h`): `AmpTime`, `CarSens`, `CarState`, `WhlSens`, `CmdSerial` (as `eepRom`), menu stack, `Lcd*` UI, `ShutDw`, optional `LpgSerial`, `AptService`.

`loop()` order matters — do not reorder casually: amplitude → melody → carSens → whlSens → shutDown → buttons/menu → car state → LPG/fuel → LCD draw → EEPROM listener → aptService.

## Coding conventions

- Prefer **header-only** modules included once via `InitObj` / `InitLcd`. Watch include order and circular deps.
- Prefer `AmpTime` flags (`isSec()`, `isRfr()`, …) over ad-hoc `delay` in listeners.
- Use **EnableInterrupt** for RPM/VSS/ECU (pins 2, 3, 19). Do not reassign without rewriting ISR setup.
- UI strings: `PROGMEM` + `getMsg(i)` into a **128-byte** buffer. Keep strings short.
- Avoid growing `String` usage; flash/RAM on Mega + full-buffer U8g2 is tight.
- Naming quirks (search both): file `Lcd240x64.h` / class `Lcd240x62`; file `Menu240x64.h` / class `Menu240x60`.
- Do not “normalize” historical typos in public identifiers unless fixing a real bug.

## Where to change what

| Goal | Touch |
|------|--------|
| Feature flags / vehicle tune | `MID/conf.h` |
| Pins / screen / baud / DEBUG | `MID/MID.h` |
| Trip computer math / sensors | `MID/lib/CarSens.h` (or `CAR_SENS_CUSTOM_*` blocks in `conf.h`) |
| UI layout / screens | `MID/lib/ui/Lcd240x64.h` |
| Menus | `MID/lib/Menu*.h`, `MID/lib/ui/Menu240x64.h` |
| New UI string | **all** `language/*.h` + `LcdMsgTable` in `ComChar.h` |
| LPG protocol | `MID/lib/LpgSerial.h`; prototype in `tests/Lpg*` first |
| Steering → radio | `WhlSens.h` + `WHL_RDO_*` in `conf.h` |

## Build

1. Install libraries from `libraries/*.zip` into the Arduino libraries folder.
2. Edit `conf.h` for the target car.
3. Open `MID/MID.ino`, board **Mega 2560**, upload.
4. CMake (`CmakeLists.txt`) is optional and often needs local path fixes; prefer Arduino IDE unless the toolchain is already configured.
5. `package.json` is metadata only — `npm test` is not a firmware test suite.

## Hard constraints

- **AmpTime** constants affect sensing **and** trip math — not cosmetic.
- Fuel IFC/CNS, VSS/RPM/DST corrections change board-computer results; document calibration impact.
- LPG modes: benzine only; `LPG_INSTALLATION`; and/or `ADT_FUEL_SYSTEM_SERIAL` / `LPG_SWTC_PIN`. Match `DEFAULT_FUEL_USING` to the install.
- Shutdown pins (`SAV_PIN_*`) save trip data on ignition off — do not break without intent.
- Serial inject (`SERIAL_INJECT_DATA`) can rewrite trip totals — dangerous on a live car.
- Display pin map is inverted relative to the LCD socket (documented in `MID.h`).
- Do not commit unzipped contents under `libraries/*/`.

## Tests folder

Standalone sketches for hardware/protocol bring-up (`LpgSerial`, `LpgDump`, `Melody`, `GmPong`, …). Use them to validate sensors/LPG before integrating into the full MID sketch. Logs live under `tests/_logs/`.

## Agent defaults

1. Prefer minimal diffs; match existing style (dense wiring comments, `const uint8_t` pins, PROGMEM strings).
2. Configure via `conf.h` before forking `MID.h`.
3. Never change AmpTime / fuel / speed corrections without noting calibration impact.
4. New messages: update every language file and `LcdMsgTable`.
5. For LPG/serial work, prefer `tests/` sketches before changing production listeners.
6. Wiki is the user-facing manual; in-tree truth for agents is `MID.ino`, `MID.h`, `conf.h`, `InitObj.h`, `InitLcd.h`.
