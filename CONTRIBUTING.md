# Contributing to FWE

Thank you for your interest in contributing to Flow Works Engine.

FWE is community-built. The goal is a simulation platform that is genuinely free, genuinely powerful, and built to last. Every contribution matters.

---

## Ways to Contribute

### Code
- **C++ core** — sub-main architecture, IPC, scene graph, state management, panic function
- **Solver integrations** — OpenFOAM, CalculiX, FEniCS, SU2 bindings
- **UI** — Dear ImGui panels, Qt outer shell
- **Python scripting layer** — API surface, materials pipeline, tooling scripts

### Data
- **Material submissions** — validated property data to `fwe-materials`
- **Environment presets** — to `fwe-environments`
- **Speaker models** — to `fwe-speakers`
- **Microphone / HRTF profiles** — to `fwe-microphones`

### Nodes
- Community node contributions go to `fwe-nodes`
- Each node must include: parameter schema, tier (1 or 2), backend dependency, and a test fixture

### Docs
- Architecture documentation
- Tutorial content
- API reference

---

## Ground Rules

### Code Quality
- C++17 minimum. No exceptions in hot paths.
- Every public function documented.
- Tests required for anything touching file I/O or solver handoff.
- No dynamic material behavior at runtime — all lookups, no live derivation.

### Material Data
- Primary sources only. Every value must cite original paper/report/institution.
- GPLv3-compatible sources only. No Engineering Toolbox, no MatWeb.
- Confidence tier required per property: `static`, `derived`, or `null`.
- Null is honest. A fabricated value is not acceptable.

### File Formats
- All `.fwe-*` formats are zip containers.
- Open tier (`.fwe-mat`, etc.) is TOML/JSON, human-readable, GPLv3.
- Format schema lives in `docs/formats/`. Any format change requires a schema version bump.

### Commits
- Descriptive commit messages. One logical change per commit.
- Branch naming: `feature/`, `fix/`, `docs/`, `node/`
- PRs must reference an issue.

---

## Pull Request Process

1. Fork the repo and create your branch from `main`
2. Make your changes with tests
3. Ensure `cmake --build` passes
4. Open a PR with a clear description of what and why
5. A maintainer will review — expect feedback, not just approval

---

## Issue Templates

Use the issue templates in `.github/ISSUE_TEMPLATE/` for:
- Bug reports
- Feature requests
- Material data submissions
- Node submissions

---

## Code of Conduct

Be direct. Be honest. Be kind. Critique code, not people.

---

*LazyR3nR3nOpenSoft*
