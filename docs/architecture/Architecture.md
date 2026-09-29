# SwasthEdge — System Architecture

**Project:** SwasthEdge — A Low-Cost Self-Diagnosing Edge Controller for Industrial Sensor Reliability  
**MCU:** STM32F446RE (Cortex-M4, 180 MHz, 512 KB Flash, 128 KB RAM)  
**Version:** 1.0  
**Date:** September 2026

---

## Overview

SwasthEdge monitors three independent health signals — sensor health, electrical health, and controller health — fuses them into a single fault-confidence score, and drives a four-state local response. The system runs on-device TinyML and operates without cloud connectivity.

The architecture is organized into five layers:

1. **Evidence Sources** — Sensor, Electrical, Controller Health
2. **Processing** — Feature Extraction → TinyML → Health Fusion
3. **Decision** — State Machine
4. **Local Action** — Display, Response, Logging
5. **Infrastructure** — Communication, External Watchdog

---

## Block Descriptions

### 1. Sensor Health

| Attribute | Detail |
| **Sensor** | SHT31 (temperature + humidity) |
| **Interface** | I2C, address 0x44 |
| **Purpose** | Provides primary sensor evidence for drift, disconnection, and out-of-range detection |
| **Methods** | EWMA (α = 0.1), CUSUM (k = 0.5σ, h = 5σ), range checks, disconnect detection |
| **Output** | Sensor evidence vector (10 values) |

**What it detects:**
- Gradual drift from baseline
- Sudden disconnection (no I2C response for 3 consecutive reads)
- Out-of-range values (temperature outside -20°C to +80°C, humidity outside 0–95%)

---

### 2. Electrical Health

| Attribute | Detail |
| **IC** | INA226 (16-bit, I2C) |
| **Interface** | I2C, address 0x40 |
| **Purpose** | Provides electrical evidence for voltage, current, and power anomalies |
| **Methods** | Shunt (0.1Ω) for current, voltage divider for bus voltage, threshold analysis |
| **Output** | Electrical evidence vector (11 values) |

**What it detects:**
- Over-voltage (>14V)
- Under-voltage (<10V)
- Over-current (>150% of baseline)
- Abnormal power consumption

---

### 3. Controller Health

| Attribute | Detail |
| **Source** | Internal MCU diagnostics |
| **Purpose** | Provides controller evidence for temperature, resets, watchdog, and communication anomalies |
| **Methods** | Internal temperature sensor, reset count (RTC backup register), watchdog status, free RAM, stack usage, communication error rate |
| **Output** | Controller evidence vector (8 values) |

**What it detects:**
- MCU overheating (>70°C warning, >85°C critical)
- Unexpected resets
- Watchdog timeouts
- Memory exhaustion
- Communication errors

---

### 4. Feature Extraction

| Attribute | Detail |
| **Input** | Sensor + Electrical + Controller evidence vectors |
| **Output** | 24 features × 10 timesteps = 240 values |
| **Window** | 10 samples (1 second at 10 Hz) |

**Features per channel (9 channels):**
- Mean
- Variance
- Standard deviation
- Slope (linear regression)
- RMS
- EWMA
- CUSUM
- Rate of change
- Min, Max, Range
- Cross-correlation (between sensor and power)

**Purpose:** Compresses raw data into a compact, informative feature vector for the TinyML model.

---

### 5. TinyML Inference

| Attribute | Detail |
| **Model** | Autoencoder + Random Forest |
| **Framework** | TensorFlow Lite Micro |
| **Quantization** | int8 |
| **Input** | 240 values (24 features × 10 timesteps) |
| **Output** | Anomaly score (0–1) + Fault class (0–7) |
| **Flash Usage** | <80 KB |
| **RAM Usage** | <24 KB |
| **Inference Latency** | <100 ms |

**Fault classes:**
- 0 = Normal
- 1 = Sensor Drift
- 2 = Sensor Disconnect
- 3 = Voltage Sag
- 4 = Abnormal Current
- 5 = Communication Failure
- 6 = Thermal Anomaly
- 7 = Multiple Faults

**Purpose:** Provides a learned anomaly detector that complements the statistical and rule-based evidence.

---

### 6. Health Fusion

| Attribute | Detail |
| **Inputs** | Sensor evidence (10) + Electrical evidence (11) + Controller evidence (8) + TinyML score (1) |
| **Output** | Fault confidence (0–100%) |
| **Weights** | Sensor: 0.35, Electrical: 0.25, Controller: 0.15, TinyML: 0.25 |
| **Agreement Bonus** | If ≥2 sources agree, confidence × 1.2 |
| **Disagreement Penalty** | If sources disagree, confidence × 0.8 |

**Purpose:** Combines independent evidence sources into a single confidence score that drives the state machine. Fusion reduces false alarms and improves fault confirmation.

---

### 7. State Machine

| State | Confidence | LED | Response |
| **Normal** | 75–100% | Green | Continue operation |
| **Warning** | 50–75% | Yellow | Log, indicate on display |
| **Degraded** | 25–50% | Orange | Isolate faulty sensor, use fallback |
| **Critical** | 0–25% | Red | Trip relay, disconnect load, safe state |

**Transition rules:**
- Normal → Warning: confidence < 75% for 3 consecutive readings
- Warning → Degraded: confidence < 50% for 3 consecutive readings
- Degraded → Critical: confidence < 25% for 3 consecutive readings
- Critical → Degraded: confidence > 35% for 30 seconds
- Degraded → Warning: confidence > 60% for 30 seconds
- Warning → Normal: confidence > 85% for 30 seconds

**Hysteresis:** 10% band between enter and exit thresholds.

**Purpose:** Provides graceful degradation and prevents oscillation.

---

### 8. Display

| Attribute | Detail |
| **Display** | 0.96" OLED (I2C, 128×64) |
| **Content** | State, confidence, active fault, evidence summary |
| **LEDs** | 5 LEDs: Power (blue), Normal (green), Warning (yellow), Degraded (orange), Critical (red) |

**Purpose:** Provides local, instant visibility without needing a laptop or cloud dashboard.

---

### 9. Response

| Attribute | Detail |
| **Actuation** | 5V SPDT relay (10A contact rating) |
| **Driver** | ULN2003 Darlington array |
| **Isolation** | Opto-isolator (4N35) |
| **Feedback** | Relay sense contact on GPIO |

**Actions per state:**
- Normal: No action
- Warning: Log, indicate
- Degraded: Isolate sensor, use fallback value
- Critical: Trip relay, disconnect load, safe state

**Purpose:** Converts detection into action — prevents damage, protects equipment, ensures safety.

---

### 10. Logging

| Attribute | Detail |
| **Primary Storage** | microSD card (SPI, 8GB) |
| **Backup Storage** | W25Q64 external flash (SPI, 8MB) |
| **RTC** | DS3231 (I2C, ±2ppm accuracy) |
| **Format** | CSV with timestamp, state, confidence, fault type, evidence |
| **Rate** | 1 row per second |
| **Retention** | 30 days on 8GB card |

**Purpose:** Stores diagnostic evidence locally for later review. Works without cloud connectivity.

---

### 11. Communication (RS485 Modbus RTU)

| Attribute | Detail |
| **Protocol** | RS485, Modbus RTU |
| **Transceiver** | MAX13487 (isolated) |
| **Baud Rate** | 9600 / 19200 / 115200 (configurable) |
| **Nodes** | Up to 247 on one bus |
| **Distance** | Up to 1200 m |
| **Gateway** | PC or Raspberry Pi |
| **Data Format** | JSON or binary (configurable) |

**Purpose:** Enables fleet aggregation. Diagnostic events are transmitted to a gateway, which collects data from multiple nodes for fleet-level visibility.

---

### 12. External Watchdog (TPS3813)

| Attribute | Detail |
| **IC** | TPS3813K33 |
| **Timeout** | 100 ms |
| **Feed Rate** | Every 50 ms (MCU toggles GPIO) |
| **Reset Pulse** | 200 ms |
| **Reset Reason** | Stored in RTC backup register |

**Connection:**
- MCU PA1 → TPS3813 WDI (heartbeat)
- TPS3813 RESET → MCU NRST (reset)
- 3.3V, GND

**Purpose:** Hardware safety net. If the MCU firmware hangs, the watchdog resets it. The MCU cannot detect its own crash — the external watchdog can.

---

## Interface Summary

| Bus | Pins | Devices |
| **I2C1** | PB8 (SCL), PB9 (SDA) | SHT31, INA226, OLED, DS3231 |
| **SPI1** | PA5 (SCK), PA6 (MISO), PA7 (MOSI) | microSD, W25Q64, MCP4131 |
| **USART1** | PA9 (TX), PA10 (RX), PA8 (DE) | RS485 (MAX13487) |
| **USART2** | PA2 (TX), PA3 (RX) | Debug (ST-Link VCP) |
| **GPIO Out** | PB3, PB4, PB5, PC7, PA0, PA1, PC1, PC6, PC8, PC9 | Relay, LEDs, watchdog, fault injection |
| **GPIO In** | PB6, PB2 | Relay feedback, SD detect |
| **ADC** | PC0 | Battery sense (optional) |

---

## Power Budget

| Rail | Voltage | Max Current | Consumers |
| **12V** | 12V | 1A | Input, relay, load |
| **5V** | 5V | 1A | Relay, OLED, SD |
| **3.3V** | 3.3V | 500 mA | MCU, sensors, RTC |

---

## Design Rationale

- **Three-layer fusion** was chosen because single-source monitoring causes false alarms and misses faults.
- **STM32F446RE** was chosen for its 180 MHz clock, 512 KB Flash, 128 KB RAM, FPU, and DSP instructions — enough for TinyML.
- **I2C for sensors** simplifies wiring and allows multiple devices on one bus.
- **SPI for storage** provides high-speed data transfer for logging.
- **RS485 for fleet aggregation** is an industrial standard, works over long distances, and supports multiple nodes.
- **External watchdog** is essential because the MCU cannot detect its own crash.
- **Local response** ensures safety without waiting for cloud connectivity.

---

## Revision History

| Version | Date | Changes |
| 1.0 | 28 Sep 2026 | Initial architecture documentation |

---

## Related Documents

- [Architecture Diagram (PDF)](SwasthEdge_Architecture.pdf)
- [Architecture Diagram (PNG)](SwasthEdge_Architecture.png)


---
