# FWE Architecture

## Overview

FWE is built around a **sub-main architecture**. The core is a lightweight orchestrator — it owns nothing heavy and brokers everything. Each tab (Model, Material, Environment, Simulate) runs as an isolated subprocess.

```
FWE Core (lightweight orchestrator)
├── Model sub-main        → OpenCASCADE, viewport, geometry
├── Material sub-main     → material library, node canvas, physics properties
├── Environment sub-main  → scene-scale sim, always-on, audio/lighting/atmosphere
└── Simulate sub-main     → component-scale precision sim, Build→Run two-stage
```

---

## FWE Core

FWE core owns:
- **Scene graph** — implemented as a temp file system using the `.fwe` format family
- **Translation layer** — converts `.fwe-objphys` + atmospheric params into solver-specific input formats at Build step
- **IPC broker** — all sub-main communication routes through core. Sub-mains never reach each other's memory directly
- **State management** — VirtualBox-style snapshots per object, per tab, per session
- **Panic function** — program-wide hard interrupt, kills all sub-mains simultaneously
- **Cross-tab dependency detection** — if Model changes after Simulate has read it, prompts "model changed — rebuild sim?"

### IPC

| Path | When | Method |
|---|---|---|
| Cold path | Tab switches | Full context serialization to temp `.fwe` files |
| Hot path | Keep-alive contexts | Shared memory IPC / memory-mapped files |

---

## Sub-Main: Model

- Geometry kernel: **OpenCASCADE** (parametric + direct modeling simultaneously)
- Section groups defined here, carry forward to all other tabs automatically
- On tab switch: core serializes Model state as temp `.fwe` file — Material tab reads from that

**Import formats v1:** STEP, IGES, OBJ, glTF/GLB, .blend, STL, DXF

---

## Sub-Main: Material

- Reads Model state from temp `.fwe` file via core
- `.fwe-objphys` bake triggered on manual save or tab switch — core handles the write
- Material properties auto-populated from `.fwe-mat` library — no dynamic calculation at runtime

**Material categories:** Mechanical, Thermal, Acoustic, Optical, Fluid, Electrical, Magnetic

---

## Sub-Main: Environment

- **Always-on continuous** simulation — no explicit Build step
- Scene-scale: entire scene runs live simultaneously
- FWE core manages via temp filing per session and per edit
- Panic function covers Environment always-on engine explicitly

**Two loading modes:**
1. Full scene
2. `.fwe-env-main` preset + one model

---

## Sub-Main: Simulate

- Component-scale precision simulation
- Data-first output: graphs, numbers, exportable logs
- **Two-stage execution:** Build → Run
- Run unlocks only after Build completes successfully

**Build step (geometry pipeline):**
```
OpenCASCADE BREP
  → Gmsh Python API (mesh generation)
    → OpenFOAM: .msh → gmshToFoam → polyMesh
    → CalculiX: .inp directly
    → SU2: .su2 directly
```

**Solver routing by placed node:**
- Fluid/aero → OpenFOAM or SU2
- Structural/thermal → CalculiX or FEniCS
- Acoustic → OpenAL + Mesh2HRTF
- No node = nothing running

---

## Node System

Universal — same nodes work in Environment and Simulate tabs. Context-agnostic.

| Tier | Description | Who uses it |
|---|---|---|
| Tier 1 | Individual nodes, granular control, assigned to specific geometry | Power users, engineers |
| Tier 2 | Pre-bundled Tier 1 with sensible defaults | Beginners, fast workflows |

Tier 2 is just Tier 1 pre-bundled — same system, two entry points.

**Node-as-initializer:** Drop node → backend spins up. Remove node → backend releases. No nodes = nothing running.

---

## Panic Function

Program-wide hard interrupt. Kills:
- GPU compute (all backends)
- All simulation backends
- Render
- Audio engine
- Environment always-on sim engine

**Manual trigger:** one button  
**Auto-trigger:** hardware threshold monitoring (temperature + load)  
**On trigger:** state saved, stopping point logged, no data loss

---

## GPU Compute

Two **independent** dropdowns — viewport and simulation never compete for the same config.

| Workload | Options |
|---|---|
| Viewport | Auto / OpenGL / DLSS / FSR / XeSS |
| Simulation | Auto / CUDA / ROCm+HIP / OpenCL / CPU |

Auto detects GPU vendor on first launch and routes correctly.

---

## State Management

- **Global state history toggle** — default OFF (max performance)
- **Per sub-main toggle** — granular control per tab
- **Auto-save** — every tab switch triggers automatic context save of the departing sub-main
- **Manual snapshots** — named slots, multiple per tab
- **Project checkpoint** — entire project across all tabs
- **Tab context save** — single sub-main only

These are separate operations with separate UI surfaces.

---

## File Format Family

All formats are zip containers. Every format has three tiers:

| Tier | Suffix | Type | Who |
|---|---|---|---|
| Open | (none) | TOML/JSON, GPLv3 | Everyone |
| Commercial | `-com` | Binary, commercial license | Licensed studios/OEMs |
| Enterprise | `-ent` | Binary + encrypted, enterprise license | R&D, air-gapped |

See `docs/formats/` for full format specifications.
