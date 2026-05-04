<a id="readme-top"></a>

[![PlatformIO][platformio-shield]][platformio-url]
[![ESP32][esp32-shield]][esp32-url]
[![C++][cpp-shield]][cpp-url]
[![3D Printer][3dprint-shield]][3dprint-url]
[![License][license-shield]][license-url]

<br />
<div align="center">
  <a href="https://github.com/rogovogor17/PlayStationPortable">
    <img src="https://raw.githubusercontent.com/tandpfun/skill-icons/main/icons/Arduino.svg" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">DIY PlayStation Portable (PSP)</h3>
  <h4 align="center">ESP32 + TFT Handheld with Battle City Game</h4>

<div align="center">

**[Getting Started](#getting-started)** • **[Hardware](#hardware)** • **[Game](#game-implementation)** • **[Case](#3d-printed-case)** • **[Roadmap](#roadmap)**

</div>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#key-features">Key Features</a></li>
      </ul>
    </li>
    <li><a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#firmware-upload">Firmware Upload</a></li>
      </ul>
    </li>
    <li><a href="#hardware">Hardware</a>
      <ul>
        <li><a href="#components">Components</a></li>
        <li><a href="#schematic--pinout">Schematic & Pinout</a></li>
      </ul>
    </li>
    <li><a href="#game-implementation">Game Implementation</a>
      <ul>
        <li><a href="#controls">Controls</a></li>
        <li><a href="#game-mechanics">Game Mechanics</a></li>
        <li><a href="#class-structure">Class Structure</a></li>
      </ul>
    </li>
    <li><a href="#3d-printed-case">3D Printed Case</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
	<li><a href="#contributors">Contributors</a></li>
    <li><a href="#references">References</a></li>

  </ol>
</details>

---

## About The Project

DIY portable game console inspired by classic handhelds, built around the ESP32 microcontroller and a 480×320 TFT display. The device plays a custom Battle City‑like tank game and is enclosed in a fully 3D‑printed case. All hardware schematics, firmware, and 3D models are provided.

### Key Features

| Feature                | Description                                                                    |
| ---------------------- | ------------------------------------------------------------------------------ |
| **Custom Hardware**    | ESP32‑WROOM, ILI9488 TFT 3.5″/3.95″, 9 physical buttons, Li‑Po battery support |
| **Full Game**          | Battle City clone with tanks, enemies, walls, bonuses, and increasing levels   |
| **3D Printed Case**    | Ergonomic shell designed in CAD, printed on a standard FDM printer             |
| **PlatformIO Project** | Modern C++ (GNU++14), easy to build and upload                                 |
| **Modular Code**       | Object‑oriented design: `Tank`, `Bullet`, `Wall`, etc.                         |

![Gameplay screenshot](img/result.png 'Battle City on ESP32')

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

## Getting Started

### Prerequisites

| Tool / Hardware       | Version / Spec                  | Purpose                 |
| --------------------- | ------------------------------- | ----------------------- |
| **PlatformIO**        | Core 6.0+ / IDE VS Code         | Build & upload firmware |
| **ESP32 board**       | DOIT ESP32 DEVKIT V1 (WROOM‑32) | Main controller         |
| **TFT display**       | 3.5″/3.95″ ILI9488 SPI 480×320  | Graphics output         |
| **9 tactile buttons** | Through‑hole 6×6 mm             | Player input            |
| **USB‑UART cable**    | Type-C data cable               | Programming & power     |

### Firmware Upload

PlatformIO may be used in two different ways: as VS Code extension (recommended) or as command line interface (CLI).

#### 1. PlatformIO Core (CLI)

```bash
#Install using Python3
pip install -U platformio

#Checking PlatformIO
pio --version
```

#### 2. Open project

```bash
#Clone repo
git clone https://github.com/rogovogor17/PlayStationPortable
cd PlayStationPortable
```

#### 3. Port Settings

In file `platformio.ini` (line `upload_port`) you can add using port for your board, for example:

```ini
upload_port = COM9    			   (Windows)
upload_port = /dev/ttyUSB0  	   (Linux)
upload_port = /dev/cu.usbserial-*  (macOS)
```

If you comment the line, PlatformIO will try to identify the port automatically .

#### 4. Building and firmware

```bash
#CLI: in root directory
pio run --target upload --target monitor
```

First running may take a few minutes - PlatformIO will download necessary libraries and toolchain ESP32.

#### 5. Important notes for ESP32

> [!NOTE]
> If the firmware does not load, hold down the **BOOT** button (or press it once when the process starts) – this will put the ESP32 into programming mode.

> [!WARNING]
> When using a battery, the board power must be disconnected during flashing (to avoid conflicts).

> [!NOTE]
> After a successful boot, the game will appear on the display. If the screen is blank, check the display connection and the settings in the TFT_eSPI library's 'User_Setup.h' (the project already includes the correct configuration).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

## Hardware

### Components

| Component       | Model / Value                         | Quantity |
| --------------- | ------------------------------------- | -------- |
| Microcontroller | ESP32‑WROOM‑32 (DOIT DevKit V1)       | 1        |
| Display         | TFT 3.5″ or 3.95″, ILI9488 controller | 1        |
| Buttons         | D-Pad (4) + Fire + Start + 3 extra    | 9        |
| Battery         | 3.7 V Li‑Po                           | 2        |
| Charging module | TP4056                                | 1        |
| Miscellaneous   | Protoboard, wires, headers, Type-C    | –        |

### Schematic & Pinout

| ESP32 Pin | Function       | Connected To            |
| --------- | -------------- | ----------------------- |
| GPIO 15   | TFT CS         | Display CS              |
| GPIO 2    | TFT DC         | Display DC              |
| GPIO 4    | TFT RST        | Display RST             |
| GPIO 23   | TFT MOSI       | Display SDI/MOSI        |
| GPIO 18   | TFT SCK        | Display SCK             |
| GPIO 19   | TFT MISO       | Display SDO/MISO        |
| GPIO 17   | Button UP      | Pull‑up + button to GND |
| GPIO 33   | Button DOWN    | Pull‑up + button to GND |
| GPIO 25   | Button LEFT    | Pull‑up + button to GND |
| GPIO 19   | Button RIGHT   | Pull‑up + button to GND |
| GPIO 27   | Button FIRE    | Pull‑up + button to GND |
| GPIO 26   | Button START   | Pull‑up + button to GND |
| GPIO 12   | Button EXTRA 1 | Pull‑up + button to GND |
| GPIO 13   | Button EXTRA 2 | Pull‑up + button to GND |
| GPIO 14   | Button EXTRA 3 | Pull‑up + button to GND |
| GND       | Common ground  | All buttons, display    |
| 3.3V      | Power (logic)  | Display VCC             |

![Schematic preview](img/setup.jpg 'Schematic assemble')

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

## Game Implementation

The firmware is written in **C++ (GNU++14)** and structured as a PlatformIO project. It uses the **TFT_eSPI** library for fast SPI graphics and **Arduino framework** for GPIO handling.

### Controls

| Button      | Action                    |
| ----------- | ------------------------- |
| **D‑Pad**   | Move player tank          |
| **Fire**    | Shoot                     |
| **Start**   | Start game / Pause        |
| **Extra 1** | (reserved for future use) |
| **Extra 2** | (reserved for future use) |
| **Extra 3** | (reserved for future use) |

### Game Mechanics

- **Player tank:** moves in four directions, shoots bullets, has health & ammunition.
- **Enemy tanks:** AI bots that move randomly and shoot.
- **Walls:** brick (destructible) and steel (indestructible).
- **Base (eagle):** must be protected from enemy fire.
- **Bonuses:** star (upgrade), shovel (temporary wall), helmet (invincibility), etc. (in future)

### Class Structure

The game logic is organised into several C++ classes that encapsulate behaviour and rendering.

<details>
<summary><b>class Tank</b> (click to expand)</summary>

```cpp
class Tank {
    const size_t max_health_, max_ammunition_;
    size_t health_, ammunition_;
    size_t x_pos, y_pos;
    size_t speed_;
    TankDirection direction_ = TankDirection::UP;

    TFT_eSPI& tft_;
    TFT_eSprite* tank_sprite_;
    uint16_t background_buffer_[DEFAULT_TANK_WIDTH * DEFAULT_TANK_HEIGHT];

public:
    Tank(size_t x, size_t y, size_t health, size_t ammo, size_t speed, TFT_eSPI& tft);
    ~Tank();

    void show(void);
    void move(int x, int y);
    void set_position(size_t x, size_t y);
    void set_direction(TankDirection direction);

    size_t get_x_pos() const;
    size_t get_y_pos() const;
    size_t get_speed() const;
    size_t get_health() const;
    size_t get_ammunition() const;
};
```

</details>

Additional classes: `Bullet`, `Wall`, `EnemyTank` (inherits from `Tank`), `GameBoard`, `GameState`.

All graphics use **TFT_eSprite** buffers to avoid flickering, and the display is updated only on changed tiles to meet the ESP32’s performance limits.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

## 3D Printed Case

The console body was designed to house the ESP32 board, TFT panel, and all buttons. It consists of three parts:

- **Front shell** with cutouts for the display and buttons.
- **Back shell** with mounting cutout for the Type-C port.
- **Back cover** for getting access to batteries.

Printing settings (common for PLA):

- Layer height: 0.2 mm
- Infill: 20% grid
- Supports: for the edges of the supporting parts of the body

The model files are available in the `case/` directory.

![Case preview](img/case.png 'Case assemble')

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

## Roadmap

- [x] **Phase 1: Hardware**
  - [x] Schematic & prototype soldering
  - [x] Button & display integration

- [x] **Phase 2: Core Game**
  - [x] Player tank movement & shooting
  - [x] Map grid (12×8 cells, 40×40 px)
  - [x] Enemy AI, walls, base, bonuses

- [x] **Phase 3: Enclosure**
  - [x] 3D‑modeled case
  - [x] FDM printing and assembly

- [ ] **Phase 4: Polish**
  - [ ] Sound effects (buzzer or speaker)
  - [ ] Battery indicator & power management
  - [ ] Multiple levels and high‑score save

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

## Contributors

The project was created and developed by enthusiasts who believe in open source and the DIY spirit.

| Author              | Role           | Tasks                                                 | Contacts                                         |
| ------------------- | -------------- | ----------------------------------------------------- | ------------------------------------------------ |
| Alexander Starostin | Hardware Lead  | Circuit design and soldering                          | [GitHub](https://github.com/Alexandr26Starostin) |
| Michael Movsesyan   | Game Developer | Game design, Game code                                | [GitHub](https://github.com/aywert)              |
| Anatoliy Rogov      | 3D Designer    | Case modeling/printing, components selection/ordering | [GitHub](https://github.com/rogovogor17)         |

---

## References

1. **PlatformIO** — [platformio.org](https://platformio.org/)
2. **TFT_eSPI library** — [github.com/Bodmer/TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
3. **Inspiration** — classic Battle City (NES, 1985)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

---

<div align="center">
  <sub>© 2026 — DIY PlayStation Portable</sub>
</div>

[platformio-shield]: https://img.shields.io/badge/PlatformIO-6.0+-F58220?style=for-the-badge&logo=platformio&logoColor=white
[platformio-url]: https://platformio.org/
[esp32-shield]: https://img.shields.io/badge/ESP32-DOIT_DevKit_V1-00B0FF?style=for-the-badge&logo=espressif&logoColor=white
[esp32-url]: https://www.espressif.com/en/products/socs/esp32
[cpp-shield]: https://img.shields.io/badge/C++-GNU++14-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white
[cpp-url]: https://isocpp.org/
[3dprint-shield]: https://img.shields.io/badge/3D_Printed-FDM-orange?style=for-the-badge&logo=thingiverse&logoColor=white
[3dprint-url]: https://www.thingiverse.com/
[license-shield]: https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge
[license-url]: LICENSE.txt
