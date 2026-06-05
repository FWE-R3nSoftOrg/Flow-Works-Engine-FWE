#pragma once

/**
 * FWE Core — Lightweight Orchestrator
 *
 * Owns: scene graph, translation layer, IPC broker, temp file system,
 *       panic function, state management, cross-tab dependency detection.
 *
 * Does NOT own any heavy backends. All simulation engines live in sub-mains.
 * All sub-main communication routes through FWECore — sub-mains never
 * reach each other's memory directly.
 *
 * Panic System — Three Levels:
 *   Level 1 — Node Cascade Kill: upstream node failure propagates through
 *             dependent nodes. Sim continues on unaffected nodes.
 *   Level 2 — Subprocess Kill: kills only the offending sub-main.
 *             FWE core and all other tabs survive.
 *   Level 3 — Full Panic: program-wide hard interrupt. Kills everything.
 *             State saved at all levels — no data loss.
 */

#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace fwe {

// Forward declarations
class ModelSubMain;
class MaterialSubMain;
class EnvironmentSubMain;
class SimulateSubMain;
class SceneGraph;
class IPCBroker;
class StateManager;

// ─────────────────────────────────────────────
// Sub-main lifecycle
// ─────────────────────────────────────────────

enum class SubMain {
    Model,
    Material,
    Environment,
    Simulate,
    Export     // export subprocess — threshold set generously to avoid mid-write kills
};

enum class SubMainState {
    Dormant,    // subprocess not running, no memory held
    Active,     // running, context loaded
    KeepAlive,  // running in background while another sub-main is active
    Warning,    // approaching resource threshold — warning icon flashing
    Killed      // auto-killed by threshold breach, awaiting user restart
};

// ─────────────────────────────────────────────
// Panic system
// ─────────────────────────────────────────────

enum class PanicLevel {
    NodeCascade,   // Level 1 — upstream node failure cascades through dependents
    Subprocess,    // Level 2 — kills one sub-main, core and others survive
    Full           // Level 3 — kills everything, program-wide hard interrupt
};

struct PanicEvent {
    PanicLevel  level;
    std::string reason;
    SubMain     affectedSubMain; // relevant for Level 2
    bool        autoTriggered;   // true = hardware threshold, false = manual
};

// ─────────────────────────────────────────────
// Hardware threshold config
// ─────────────────────────────────────────────

enum class ThresholdMode {
    SystemWide,    // one set of thresholds applies to all subprocesses (default)
    PerSubprocess  // individual threshold override per tab
};

struct ResourceThresholds {
    float ramPercent  = 85.f;   // % of allocated RAM
    float cpuPercent  = 95.f;   // % CPU utilization
    float vramPercent = 90.f;   // % VRAM (viewport and sim tracked independently)
    float tempCelsius = 90.f;   // GPU/CPU temperature
};

// ─────────────────────────────────────────────
// FWE Core
// ─────────────────────────────────────────────

class FWECore {
public:
    FWECore();
    ~FWECore();

    // ── Lifecycle ──────────────────────────────
    bool initialize();
    void shutdown();

    // ── Sub-main management ────────────────────
    bool         activateSubMain(SubMain target);
    bool         setKeepAlive(SubMain target, bool enabled);
    SubMainState getSubMainState(SubMain target) const;

    // ── Context serialization (cold path IPC) ──
    bool serializeContext(SubMain source);
    bool restoreContext(SubMain target);

    // ── Panic system ───────────────────────────

    // Level 1 — node cascade. Called by node system when an upstream node fails.
    // Propagates failure through dependent nodes. Sim continues on unaffected nodes.
    void triggerNodeCascade(const std::string& failedNodeId);

    // Level 2 — subprocess kill. Kills one sub-main, core and others survive.
    // Auto-triggered when a subprocess hits its resource threshold.
    // Manual trigger available via warning icon in UI.
    void killSubprocess(SubMain target, const std::string& reason = "");
    bool restartSubprocess(SubMain target);

    // Level 3 — full panic. Kills all sub-mains simultaneously.
    // Auto-triggered by hardware emergency. Manual trigger also available.
    void triggerFullPanic(const std::string& reason = "");

    // ── Hardware monitoring ────────────────────
    void setThresholdMode(ThresholdMode mode);
    void setSystemThresholds(const ResourceThresholds& thresholds);
    void setSubprocessThresholds(SubMain target, const ResourceThresholds& thresholds);
    ResourceThresholds getSubprocessThresholds(SubMain target) const;

    // Per-subprocess resource usage — for UI warning icon
    float getRamUsagePercent(SubMain target)  const;
    float getCpuUsagePercent(SubMain target)  const;
    float getVramUsagePercent(SubMain target) const; // viewport and sim tracked independently
    float getTemperatureCelsius()             const;

    // ── State history ──────────────────────────
    void setStateHistoryEnabled(bool enabled);                          // global toggle (default OFF)
    void setStateHistoryEnabled(SubMain target, bool enabled);         // per sub-main override
    bool isStateHistoryEnabled()                     const;
    bool isStateHistoryEnabled(SubMain target)       const;

    // ── Cross-tab dependency tracking ──────────
    void notifyModelChanged();
    bool isSimulateStale() const;

    // ── Scene graph access ─────────────────────
    SceneGraph&       sceneGraph();
    const SceneGraph& sceneGraph() const;

    // ── Translation layer ──────────────────────
    // Translates .fwe-objphys + atmospheric params → solver-specific input formats.
    // Called at Build step by Simulate sub-main.
    bool buildSolverInput(SubMain solverTarget);

    // ── Callbacks ──────────────────────────────
    using PanicCallback      = std::function<void(const PanicEvent&)>;
    using SubMainKillCallback = std::function<void(SubMain, const std::string&)>;
    using DependencyCallback = std::function<void()>;
    using WarningCallback    = std::function<void(SubMain, float ramPct, float cpuPct, float vramPct)>;

    void onPanic(PanicCallback cb);
    void onSubprocessKilled(SubMainKillCallback cb);
    void onModelChangedWhileSimulateActive(DependencyCallback cb);
    void onSubprocessWarning(WarningCallback cb);  // fires when subprocess approaches threshold

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace fwe
