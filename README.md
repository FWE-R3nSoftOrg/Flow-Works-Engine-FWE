# FWE — Flow Works Engine

> All-in-one free open-source engineering design and simulation platform.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://img.shields.io/badge/build-pre--alpha-orange)]()
[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen)]()

---

## What Is FWE?

FWE lets you **model, simulate, and iterate** without ever leaving the app.

No fragmented tools. No paid licenses. The simulation power already exists in open-source engines — FWE is the layer that finally makes them human.

- **Model** any geometry — engines, speakers, rotors, airframes, anything
- **Assign** real physics and material properties
- **Simulate** — fluid, thermal, structural, acoustic, electromagnetic, and more
- **Iterate** — checkpoints, overlays, live data, exportable results

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

## Business Model

| Tier | License | Who |
|---|---|---|
| FWE core | GPLv3, free forever | Everyone |
| Community modules | GPLv3, public | Anyone builds and shares |
| Private modules | Commercial API license | Companies build internally |
| FWE API spec | Licensed commercially | Studios, OEMs, defense, integrators |
| Verified environment presets | Commercial bundle | ISO-certified, real-world characterized |
| Priority material data | Commercial bundle | Advanced composites, high-spec materials |

*`.fwe` as the industry standard for simulation-accurate 3D assets is the long-term moat.*

---

## Getting Started

> ⚠️ FWE is in **pre-alpha**. No runnable build exists yet. This repo is the foundation being actively developed.

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

FWE is community-built. Contributions are welcome across all areas:

- **C++ / simulation engineering** — core, sub-mains, solver integrations
- **Python** — scripting layer, materials pipeline, tooling
- **UI / UX** — Dear ImGui, Qt
- **Domain expertise** — acoustics, fluid dynamics, structural, thermodynamics
- **Material data** — validated property submissions to `fwe-materials`
- **Node contributions** — submit to `fwe-nodes`

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

---

## License

FWE core is licensed under **GNU General Public License v3.0**.  
See [LICENSE](LICENSE) for full terms.

Commercial API license available for enterprise integrators — see [docs/commercial-license.md](docs/commercial-license.md).

---

*LazyR3nR3nOpenSoft*
