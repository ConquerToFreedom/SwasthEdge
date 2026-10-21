# SwasthEdge: Engineering Bill of Materials (BOM)

This document outlines the complete Bill of Materials for **SwasthEdge**, covering both **Stage A (Breadboard/Nucleo Prototyping)** and **Stage B (Custom PCB Fabrication)**. 

Items are categorized by necessity to ensure cost-sensitive viability for MSME deployment.

*   **Essential:** Required for a working prototype or validation.
*   **Recommended:** Improves reliability, validation quality, or ease of debugging.
*   **Optional:** Nice to have, but can be omitted without affecting core functionality.

> **Cost-Sensitive Viability:** The total cost of essential hardware for Stage B (Custom PCB) is approximately **₹7,000 (~$85)**, proving the viability of advanced TinyML self-diagnosis for industrial retrofits.

---

## 1. Core Controller

| Item | Part Number | Qty | Necessity | Alternatives | Approx Cost (₹) | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Nucleo-F446RE dev board | NUCLEO-F446RE | 1 | **Essential** (Stage A) | Any STM32F4 Nucleo | 1,200 | Already owned by team. |
| Bare STM32F446RE IC | STM32F446RET6 | 3 | **Essential** (Stage B) | STM32F411RET6 | 550–700 each | LQFP64 package. Buy 3 (spares). |
| ST-Link V2 debugger | ST-Link V2 clone | 1 | **Essential** | On-board ST-Link | 250 | Needed for custom PCB programming. |

## 2. Sensors & Monitoring

| Item | Part Number | Qty | Necessity | Alternatives | Approx Cost (₹) | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Temp/Humidity Sensor | SHT31 (module/IC) | 1 | **Essential** | SHT30, DHT22 | 250 | I2C. For environmental health. |
| Power Monitor IC | INA226 (module/IC) | 1 | **Essential** | INA219, INA228 | 180 | I2C. Measures V, I, P. |
| Shunt resistor | 0.1Ω, 1%, 2W, 2512 | 2 | **Essential** | 0.05Ω, 0.2Ω | 30 each | For INA226 current measurement. |
| Voltage divider | 100kΩ, 10kΩ (1%) | 2 ea | **Essential** | — | 5 each | For bus voltage sensing. |
| Op-amp buffer | MCP6002 | 1 | **Recommended** | LM358 | 80 | Impedance matching. |
| RTC module | DS3231 (module/IC) | 1 | **Essential** | DS1307 | 120 | I2C. Timestamping events. |
| CR2032 battery | CR2032 | 1 | **Essential** | — | 30 | For RTC backup. |

## 3. Display & Indicators

| Item | Part Number | Qty | Necessity | Alternatives | Approx Cost (₹) | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| OLED display | 0.96" I2C SSD1306 | 1 | **Recommended** | 1.3" OLED | 300 | Shows state, confidence, fault. |
| Status LEDs | 5mm (G, Y, O, R, Pwr) | 5 | **Essential** | 3mm LEDs | 5 each | State indication. |
| LED resistors | 330Ω | 5 | **Essential** | 220Ω, 470Ω | 2 each | Current limiting. |

## 4. Storage & Logging

| Item | Part Number | Qty | Necessity | Alternatives | Approx Cost (₹) | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| MicroSD Card Module | SPI Interface | 1 | **Essential** | Direct SD slot on PCB | 150 | Local diagnostic logging. |
| SPI Flash Memory | W25Q64 | 1 | **Recommended** | W25Q32 | 80 | For fast, reliable fault logging. |

## 5. Communication

| Item | Part Number | Qty | Necessity | Alternatives | Approx Cost (₹) | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| RS485 transceiver | MAX13487 | 1 | **Essential** (Fleet) | MAX485 (non-isolated) | 250 | Isolated version preferred. |
| RS485 module (Stage A) | MAX485 module | 1 | **Essential** (Stage A) | — | 80 | For initial breadboard testing. |
| Termination resistor | 120Ω, 1/4W | 2 | **Essential** | — | 5 each | At both ends of RS485 bus. |
| Bias resistors | 680Ω | 2 | **Recommended** | — | 5 each | Ensures idle state. |
| TVS diodes | SMBJ6.5CA | 2 | **Recommended** | — | 20 each | Surge protection. |
| Terminal block | 3-pin, 5.08mm | 1 | **Essential** | DB9 | 30 | For RS485 connection. |

## 6. Watchdog & Safety

| Item | Part Number | Qty | Necessity | Alternatives | Approx Cost (₹) | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| External watchdog IC | TPS3813K33 | 1 | **Essential** | MAX6369 | 150 | Resets MCU if firmware hangs. |
| Reset pull-up | 10kΩ | 1 | **Essential** | — | 2 | — |
| Decoupling capacitor | 100nF | 2 | **Essential** | — | 2 each | For watchdog and MCU. |

## 7. Actuation & Local Response

| Item | Part Number | Qty | Necessity | Alternatives | Approx Cost (₹) | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Relay | 5V SPDT, 10A | 1 | **Essential** | 12V relay | 80 | For fault isolation / load disconnect. |
| Relay driver | ULN2003 | 1 | **Essential** | ULN2803 | 40 | Darlington array. |
| Flyback diode | 1N4007 | 1 | **Essential** | 1N4148 | 2 | Across relay coil. |
| Terminal block | 2-pin, 5.08mm | 2 | **Essential** | — | 20 each | For relay output. |

## 8. Fault Injection Rig

| Item | Part Number | Qty | Necessity | Alternatives | Approx Cost (₹) | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Relay module | 5V, 4-channel | 1 | **Essential** | 4 individual relays | 200 | For disconnect/comm failure. |
| Digital potentiometer | MCP4131 (SPI) | 1 | **Essential** | MCP4231 | 80 | For sensor drift injection. |
| MOSFET | IRLZ44N | 2 | **Essential** | IRF540N | 40 each | For voltage sag injection. |
| Power resistor | 10Ω, 10W | 1 | **Essential** | 5Ω, 20Ω | 50 | For abnormal current injection. |
| Perfboard | 10×15cm | 1 | **Essential** | — | 50 | For fault injection rig assembly. |

## 9. Custom PCB (Stage B)

| Item | Part Number | Qty | Necessity | Alternatives | Approx Cost (₹) | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| PCB fabrication | 2-layer, 120×100mm | 5 | **Essential** | 4-layer | 1,500 total | LionCircuits, JLCPCB. |
| SMD passives | 0805/1206 resistors, caps | — | **Essential** | Through-hole | 500 | For PCB assembly. |
| Connectors | Terminal blocks, headers | — | **Essential** | — | 300 | For PCB. |
| Mounting hardware | M3 screws, standoffs | 1 set | **Essential** | — | 100 | For enclosure/stand. |

---

## Priority Purchasing Schedule

| Priority | Items to Purchase |
| :--- | :--- |
| **1 (Immediate)** | Nucleo (owned), SHT31, INA226, OLED, microSD module, DS3231, relay, ULN2003, jumper wires, breadboard. |
| **2 (Week 2)** | RS485 module, MAX13487, TPS3813, MCP4131, IRLZ44N, power resistor, perfboard. |
| **3 (Week 3)** | STM32F446RET6 (3×), PCB fabrication, SMD passives, connectors. |
| **4 (Week 4)** | Enclosure, spares, tools (if not in lab). |

## Recommended Vendors (India)

| Vendor | Best For |
| :--- | :--- |
| **Mouser / Digi-Key India** | Authentic ICs (STM32, INA226, TPS3813) |
| **Robu.in** | Modules, sensors, relays, passives |
| **SP Road (Bangalore)** | Local, immediate, bargain |
| **LionCircuits / JLCPCB** | PCB fabrication |
| **Amazon** | Modules, tools (verify seller ratings) |
