# SwasthEdge KiCad project

Open `SwasthEdge/SwasthEdge.kicad_pro` with KiCad 10.

## Current project files

- `SwasthEdge/SwasthEdge.kicad_sch` — current schematic. The supplied October 22, 2026 file matches this repository file byte-for-byte.
- `SwasthEdge/SwasthEdge.kicad_pcb` — updated with the supplied `powermod_experimental_all_footprints_routed (2).kicad_pcb`, renamed to match the project.
- `SwasthEdge/SwasthEdge.kicad_pro` — existing KiCad project settings.

## Related artifacts and validation

`SwasthEdge/SwasthEdge.net`, `SwasthEdge/SwasthEdge_footprint_report.txt`, and `SwasthEdge/SwasthEdge_board_preview.png` are from the earlier October 19 snapshot and were not regenerated for the updated board. Do not use them to validate the October 22 PCB.

The PCB was supplied with “routed” in its filename, but this update does not establish that routing passes KiCad DRC or that the schematic passes ERC. Run ERC and DRC in KiCad before treating the board as fabrication-ready.
