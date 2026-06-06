# FWE — Flow Works Engine

> All-in-one free open-source engineering design and simulation platform.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://img.shields.io/badge/build-pre--alpha-orange)]()
[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen)]()
---

<img src="docs/FWElogo.png" width="200"/>

## What Is FWE?

FWE lets you **model, simulate, and iterate** without ever leaving the app.

The simulation power already exists in open-source engines — FWE is the layer that finally makes them accessible and zero hassle for average user. No fragmented tools. No modular installs where half the physics are missing.

- **Model** any geometry — engines, speakers, rotors, airframes, anything
- **Assign** real material properties with validated physical data
- **Simulate** — fluid, thermal, structural, acoustic, electromagnetic, and more
- **Iterate** — checkpoints, overlays, live data, exportable results

---

## Why FWE Exists

Every simulation tool today is either locked behind expensive licenses, fragmented across five different programs, or requires a PhD to set up. The open-source engines that could do the job — OpenFOAM, FEniCS, CalculiX, OpenAL — are powerful but inaccessible to most engineers and builders.

FWE connects them into one coherent workflow. Same project file, same interface, physics that talk to each other.

---

## Core Philosophy

| Principle | What It Means |
|---|---|
| One stop | Design and test in the same environment |
| Sub-main architecture | Each tab is its own subprocess — crash-isolated, memory-isolated |
| Node-as-initializer | Drop a node → backend spins up. Remove it → backend releases |
| Progressive disclosure | Simple on the surface, deep when you dig |
| Fast lane & Full lane | Model → Simulate for quick runs. Full pipeline for precision work |
| Everything ships complete | Simulation types are toggleable overlays, not separate installs |

---

## Tech Stack

| Layer | Technology |
|---|---|
| Geometry kernel | OpenCASCADE |
| Fluid / Aero | OpenFOAM + SU2 + Code Saturne |
| Structural / Thermal FEM | FEniCS / CalculiX / Sparselizard |
| Mechanical systems | OpenModelica |
| Mesh generation | Gmsh |
| Visualization | ParaView |
| Audio (3D spatial) | OpenAL Soft |
| HRTF calculation | Mesh2HRTF |
| Rendering | OpenGL |
| GPU — NVIDIA | CUDA |
| GPU — AMD | ROCm / HIP |
| GPU — fallback | OpenCL |
| Upscaling | DLSS / FSR / XeSS |
| Language | C++ core, Python scripting layer |
| UI | Dear ImGui + Qt |
| Build | CMake |

---

## Repository Structure

```
Flow-Works-Engine-FWE/     ← this repo (core application)
fwe-materials/             ← material data library (GPLv3)
fwe-environments/          ← environment presets
fwe-nodes/                 ← community node contributions
fwe-speakers/              ← speaker model library
fwe-microphones/           ← microphone / HRTF profiles
```

---

## Module Roadmap

```
v0.1.x  Core → Model → Material → Simulate → File Formats → Export → Settings
v0.2.0  Fluid → Thermal → Mechanical → Acoustic → Lighting → Vibration
v0.3.0  Automotive → Marine → Aerospace → Drone / Small Systems
v0.4.0+ Advanced Materials → Ballistics → Instrument / VST pipeline
```

---

## How FWE Stays Free

FWE core is GPLv3, free forever. A commercial API license exists for organizations that want to integrate the `.fwe` format into their own products or build private modules — that's what funds continued development. It's invisible to 99% of users.

| Tier | License |
|---|---|
| FWE core | GPLv3, free forever |
| Community modules | GPLv3, public |
| Private modules | Commercial API license |
| FWE API spec + verified data | Commercial bundle |

Individual users always work in the open `.fwe` format — no keys, no barriers, no lock-in.

---

## Getting Started

FOR ALL FUTURE CONTRIBUTORS

> ⚠️ FWE is still a concept WITH ARCHITECTURAL DESIGN DOCS, please go to: https://github.com/LazyR3nR3nOpenSoft/FWEvision-direction for vision and direction and more structural details. This repo is the foundation being actively developed.

### Prerequisites

- CMake 3.20+
- C++17 compiler (GCC 11+ / Clang 13+ / MSVC 2022+)
- Python 3.10+
- OpenCASCADE 7.6+

### Build (when available)

```bash
git clone https://github.com/LazyR3nR3nOpenSoft/Flow-Works-Engine-FWE.git
cd Flow-Works-Engine-FWE
mkdir build && cd build
cmake ..
make -j$(nproc)
```

---

## Contributing

FWE is community-built. The core is C++ but contributions are welcome across everything:

- **C++ / simulation engineering** — core architecture, sub-mains, solver integrations
- **Python** — scripting layer, materials pipeline, build tooling
- **UI / UX** — Dear ImGui, Qt
- **Domain expertise** — if you know acoustics, fluid dynamics, structural, thermodynamics, or any simulation domain deeply, that knowledge matters as much as the code
- **Material data** — validated property submissions to `fwe-materials`
- **Node contributions** — new node types submitted to `fwe-nodes`

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

---

#Support

If you want to support FWE, you can here:


☕ [Ko-fi — LazyR3nR3n](https://ko-fi.com/lazyr3nr3n)



---
## License

FWE core is licensed under **GNU General Public License v3.0**.  
See [LICENSE](LICENSE) for full terms.

Commercial API license available for integrators building on the `.fwe` format — see [docs/commercial-license.md](docs/commercial-license.md).

---

#Support

If you want to support FWE, you can here:


☕ [Ko-fi — LazyR3nR3n](https://ko-fi.com/lazyr3nr3n)



---


*LazyR3nR3nOpenSoft*
