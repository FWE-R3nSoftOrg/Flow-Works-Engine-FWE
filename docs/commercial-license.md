# FWE Commercial License

## Overview

FWE core is GPLv3, free forever. The commercial license is for organizations that need to integrate FWE's file formats into their own products, or require enterprise-grade security for R&D workflows.

*NOTE: this can still be remove or change base on community request. this is just so the development sake and decisions will vary if contributors request it to be removed, You- the contributors have the decision I, only have the vision. -LazyR3nR3n*

---

## Tiers

### Community — Free

- Full FWE application, all simulation features
- `.fwe` open format family (TOML/JSON, GPLv3)
- Community node and material contributions
- No license key required

### Commercial API License

- Binary `.fwe-*-com` format family read/write
- FWE API spec access — integrate `.fwe` formats into your own pipeline
- SDK for external software integration
- Private node building rights
- Org-shared library access
- Seat-limited, files shareable between licensed installs for collaboration

Target: game studios, OEMs, software integrators, Unity/Unreal pipelines

### Enterprise R&D License

- Everything in Commercial
- `.fwe-*-ent` encrypted format family — org-specific ECC encryption
- Files unreadable without your org's private key
- Air-gapped operation — no network dependency
- Zero-knowledge guarantee: we cannot access your files
- Extended migration windows on major version updates
- Enterprise support

Target: R&D organizations where IP leakage is unacceptable

---

## How It Works

No separate installer. No different binary. Same FWE everyone downloads.

**Settings → Version/License → paste your API key → capabilities unlock.**

The commercial layer is invisible to community users. If you don't have a key, you don't see it. The Settings license field doesn't appear in community mode.

---

## Inquiries

For commercial or enterprise licensing:
→ Open an issue tagged `[LICENSE]` or contact via GitHub.

*(Direct contact details will be added at launch.)*
