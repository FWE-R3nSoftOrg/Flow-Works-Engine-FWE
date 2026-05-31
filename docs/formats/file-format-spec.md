# FWE File Format Specification

All `.fwe-*` formats are **zip containers**. Each carries exactly what it needs — no more, no less. Typed assets mean clean imports: a file either loads fully or doesn't load at all. No half-broken states.

---

## Format Family

| Format | Contains | Who uses it |
|---|---|---|
| `.fwe` | Full project — geometry, sim data, checkpoints, all metadata | Everyone — the master project file |
| `.fwe-env-main` | Base environment only — atmosphere, boundary conditions, environment geometry + materials | Preset makers, community sharing, stage teams |
| `.fwe-env-scene` | Full session — references `.fwe-env-main` + all models, nodes, measurement data, sim state | Engineers saving full working sessions |
| `.fwe-obj` | Geometry only — mesh, dimensions, section groups | Aero/hydro engineers, game devs |
| `.fwe-objphys` | Geometry + material properties + physics metadata | R&D, structural, thermal, acoustic |
| `.fwe-mat` | Material source data — TOML, human readable, never loaded at runtime | Material library, community contributions |
| `.fwe-spkmod` | Speaker model — Thiele-Small params, enclosure geometry, driver profile | Speaker manufacturers, acoustic engineers |
| `.fwe-litmod` | Light fixture model — beam geometry, intensity/color profile, DMX mapping | Lighting manufacturers, stage designers |
| `.fwe-theme` | UI theme — color scheme, panel styling | Community theme sharing |
| `.fwe-set-pref` | Full settings configuration | Teams, enterprise IT |

---

## Three-Tier System

Every format has three tiers. Applied as a suffix:

| Tier | Suffix example | Type | License |
|---|---|---|---|
| Open | `.fwe-mat` | TOML/JSON, human-readable | GPLv3, free |
| Commercial | `.fwe-mat-com` | Proprietary binary | Commercial API license |
| Enterprise | `.fwe-mat-ent` | Binary + ECC encrypted | Enterprise license |

This applies to every format in the family:
`.fwe-obj` / `.fwe-obj-com` / `.fwe-obj-ent`
`.fwe-env-main` / `.fwe-env-main-com` / `.fwe-env-main-ent`
etc.

---

## Zip Container Structure

Every `.fwe-*` file is a zip with a defined internal layout:

```
[format].fwe-*/
├── manifest.json       ← format version, type, creation metadata
├── data/               ← primary payload (TOML for open tier, binary for com/ent)
├── meta/               ← optional: thumbnail, description, tags
└── checksums.sha256    ← integrity verification
```

### manifest.json

```json
{
  "fwe_format": "fwe-mat",
  "format_version": "1.0",
  "fwe_version": "0.1.0",
  "created": "2025-01-01T00:00:00Z",
  "author": "",
  "license_tier": "open"
}
```

---

## `.fwe-mat` — Material Format (Open Tier)

TOML. Human-readable. Primary source of truth for all material data.
Never loaded at runtime directly — values are baked into `.fwe-objphys` at save time.

### Confidence Tiers

Every property carries a confidence tag:

| Tag | Meaning | Sim behavior |
|---|---|---|
| `static` | Value pulled directly from a cited primary source | Runs silently |
| `derived` | Computed offline from other properties, formula cited | Soft warning logged |
| `null` | No source and no derivation path | FWE prompts user at sim time |

### Schema

```toml
[meta]
name        = "Titanium Aluminide (γ-TiAl)"
formula     = "TiAl"
category    = "Metal"
source      = "Materials Project mp-1953"
doi         = "10.17188/1194736"

[mechanical]
density           = { value = 3870.0, unit = "kg/m³",  confidence = "static",  source = "mp-1953" }
youngs_modulus    = { value = 170e9,  unit = "Pa",      confidence = "derived", formula = "2G(1+ν)" }
shear_modulus     = { value = 68e9,   unit = "Pa",      confidence = "static",  source = "mp-1953" }
bulk_modulus      = { value = 113e9,  unit = "Pa",      confidence = "static",  source = "mp-1953" }
poissons_ratio    = { value = 0.25,   unit = "",        confidence = "static",  source = "mp-1953" }

[thermal]
thermal_expansion_a = { value = 11e-6,  unit = "K⁻¹", confidence = "static", source = "Holec et al. 2019" }
thermal_expansion_c = { value = 5e-6,   unit = "K⁻¹", confidence = "static", source = "Holec et al. 2019" }
specific_heat       = { value = 668.0,  unit = "J/kg·K", confidence = "static", source = "Holec et al. 2019" }
thermal_conductivity = { value = 0.0,   unit = "W/m·K", confidence = "null",   note = "Range 7-25 W/m·K, exact value behind paywall" }

[acoustic]
# Derived from mechanical properties — baked at pipeline build step
speed_of_sound_longitudinal = { value = 6120.0, unit = "m/s", confidence = "derived", formula = "sqrt((K + 4G/3) / ρ)" }
speed_of_sound_shear        = { value = 4195.0, unit = "m/s", confidence = "derived", formula = "sqrt(G / ρ)" }
damping_loss_factor         = { value = 0.0,    unit = "",    confidence = "null" }

[electrical]
classification  = "Conductor"
conductivity    = { value = 0.0, unit = "S/m", confidence = "null" }

[magnetic]
classification  = "Paramagnetic"
permeability    = { value = 1.0, unit = "H/m", confidence = "derived", formula = "relative_permeability × μ₀" }
```

---

## `.fwe-objphys` — Physics Object Format

Geometry + material properties baked together. One file read, one memory allocation, no runtime cross-referencing.

Material swapping happens at export/save time in the Material tab, not at runtime. `.fwe-mat` is the source library — it is never loaded at runtime directly.

Binary layout optimized for solver read order — properties packed in the sequence that the sim engine requests them, minimizing cache misses.

---

## `.fwe-env-main` vs `.fwe-env-scene`

**`.fwe-env-main`** — base environment only.
- Atmospheric medium, boundary conditions, environment geometry and surface materials
- Reusable, shareable, context-agnostic
- What ships as presets (anechoic chamber, wind tunnel, still water, etc.)

**`.fwe-env-scene`** — full working session.
- References a `.fwe-env-main` + all placed models and transforms + all placed nodes and configs + measurement node data and logs + simulation state at save point
- Embeds into the master `.fwe` project file

---

## Licensing Architecture

### License Key — Capability Manifest

Fully offline. No phone home. No license server. Works air-gapped.

Each key encodes:
- Which file tiers it can decrypt (`.fwe-*-com`, `.fwe-*-ent`, or both)
- Feature unlocks
- Seat count
- Hardware fingerprint slots (similar to Windows activation)

### Commercial (`.fwe-*-com`)
- Shareable between any commercially licensed FWE installs
- Seat-limited key
- Binary format — readable by any licensed install

### Enterprise (`.fwe-*-ent`)
- Org-specific ECC encryption
- Files unreadable without the org's private key
- Air-gapped — no sharing outside the org
- Zero-knowledge guarantee: LazyR3nR3nOpenSoft cannot open enterprise files

### Encryption
ECC (Elliptic Curve Cryptography) — smaller keys, faster operations, equivalent security to RSA at larger sizes. Appropriate for embedded key architecture.
