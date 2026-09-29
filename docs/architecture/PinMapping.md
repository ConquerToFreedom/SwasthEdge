# SwasthEdge — Complete Pin Mapping (STM32F446RE / Nucleo-F446RE)

**MCU:** STM32F446RE (LQFP64)  
**Board:** Nucleo-F446RE  
**Version:** 1.0  
**Date:** September 2026

---

## Complete Pin Map

| STM32 Pin | Function | Bus / Type | Arduino | Direction | Notes |
|---|---|---|---|---|---|
| **PB8** | I2C1_SCL | I2C | D15 | Output | SHT31, INA226, OLED, DS3231 |
| **PB9** | I2C1_SDA | I2C | D14 | Bidirectional | SHT31, INA226, OLED, DS3231 |
| **PA5** | SPI1_SCK | SPI | D13 | Output | microSD, W25Q64, MCP4131 |
| **PA6** | SPI1_MISO | SPI | D12 | Input | microSD, W25Q64, MCP4131 |
| **PA7** | SPI1_MOSI | SPI | D11 | Output | microSD, W25Q64, MCP4131 |
| **PA4** | CS_SD | SPI (CS) | A2 | Output | microSD chip select |
| **PB0** | CS_FLASH | SPI (CS) | A3 | Output | W25Q64 chip select |
| **PB10** | CS_POT | SPI (CS) | D6 | Output | MCP4131 chip select |
| **PA9** | USART1_TX | UART | D8 | Output | RS485 TX → MAX13487 DI |
| **PA10** | USART1_RX | UART | D2 | Input | RS485 RX ← MAX13487 RO |
| **PA8** | RS485_DE | GPIO | D7 | Output | RS485 direction control |
| **PA2** | USART2_TX | UART | D1 | Output | Debug TX (ST-Link VCP) |
| **PA3** | USART2_RX | UART | D0 | Input | Debug RX (ST-Link VCP) |
| **PB3** | RELAY_MAIN | GPIO | D3 | Output | Main relay for local response |
| **PB4** | LED_GREEN | GPIO | D4 | Output | Normal state indicator |
| **PB5** | LED_YELLOW | GPIO | D5 | Output | Warning state indicator |
| **PC7** | LED_ORANGE | GPIO | D9 | Output | Degraded state indicator |
| **PA0** | LED_RED | GPIO | A0 | Output | Critical state indicator |
| **PA1** | WDI | GPIO | A1 | Output | External watchdog feed (50 ms) |
| **PC1** | FAULT_DISCONNECT | GPIO | A4 | Output | Relay for sensor disconnect |
| **PC6** | FAULT_SAG_PWM | TIM3_CH1 | Morpho CN10 | Output | MOSFET PWM for voltage sag |
| **PC8** | FAULT_CURRENT | GPIO | Morpho CN10 | Output | Relay for abnormal current |
| **PC9** | FAULT_COMM | GPIO | Morpho CN10 | Output | Relay for communication failure |
| **PB6** | RELAY_FEEDBACK | GPIO | D10 | Input | Relay sense contact |
| **PB2** | SD_DETECT | GPIO | Morpho CN10 | Input | SD card detect (optional) |
| **PC0** | VBAT_SENSE | ADC | A5 | Input | Battery voltage (optional) |
| **NRST** | RESET | System | Morpho CN7 | Input | External watchdog reset output |
| **PA13** | SWDIO | Debug | On-board | Bidirectional | Programming / debug |
| **PA14** | SWCLK | Debug | On-board | Input | Programming / debug |

---

## Device Address Map (I2C1)

| Device | Address | Purpose |
|---|---|---|
| SHT31 | 0x44 | Temperature + humidity |
| INA226 | 0x40 | Voltage, current, power |
| OLED (SSD1306) | 0x3C | Display |
| DS3231 | 0x68 | Real-time clock |

---

## SPI Chip Select Assignments

| Device | CS Pin | Speed | Purpose |
|---|---|---|---|
| microSD | PA4 | 10 MHz | Primary logging |
| W25Q64 | PB0 | 10 MHz | Backup logging |
| MCP4131 | PB10 | 1 MHz | Sensor drift injection |

**Rule:** Only one CS low at a time.

---

## Timer Assignments

| Timer | Channel | Pin | Purpose | Frequency |
|---|---|---|---|---|
| TIM3 | CH1 | PC6 | MOSFET PWM (voltage sag) | 1–10 kHz |
| TIM6 | — | — | Sampling timer | 10 Hz |
| TIM7 | — | — | Inference timer | 1 Hz |
| TIM14 | — | — | Watchdog feed timer | 20 Hz (50 ms) |

---

## Pin Conflict Verification

| Check | Result |
|---|---|
| I2C1 (PB8/PB9) vs. SPI1 (PA5/PA6/PA7) | No conflict |
| I2C1 vs. USART1 (PA9/PA10) | No conflict |
| I2C1 vs. USART2 (PA2/PA3) | No conflict |
| SPI1 vs. USART1 | No conflict |
| PA4, PB0, PB10 (CS pins) vs. SPI1 | No conflict (CS = GPIO) |
| PC6 (PWM) vs. TIM3 | Correct alternate function |
| PA0, PA1 (GPIO) vs. TIM2 | Configured as GPIO, not timer |
| All GPIO pins | No overlap |

**Verdict:** No pin conflicts.

---

## Power Pins

| Rail | Voltage | Max Current | Location |
|---|---|---|---|
| 12V | 12V | 1A | External input |
| 5V | 5V | 1A | Arduino + Morpho headers |
| 3.3V | 3.3V | 500 mA | Arduino + Morpho headers |
| GND | 0V | — | Multiple pins |

---

