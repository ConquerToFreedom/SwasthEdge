# SwasthEdge
Low-cost self-diagnosing edge controller for industrial sensor reliability

## Overview
- Monitors sensor, electrical, and MCU health
- Fuses evidence into a fault-confidence score
- On-device TinyML on STM32F446RE
- Local response: Normal → Warning → Degraded → Critical
- Physical fault injection validation

## Repository Structure
- `/firmware` – STM32 firmware
- `/hardware` – KiCad schematic, PCB layout, BOM
- `/ml` – Data, notebooks, models
- `/docs` – Architecture, validation protocol, reports
- `/ppt` – Presentation slides
- `/video` – Demo video links

## Documentation
- [System Architecture](docs/architecture/Architecture.md)
- [Architecture Diagram (PDF)](docs/architecture/SwasthEdge_Architecture.pdf)
- [Complete Pin Mapping (Markdown)](docs/architecture/PinMapping.md)
- [Pin Mapping (CSV for Excel)](docs/architecture/PinMapping.csv)
  

