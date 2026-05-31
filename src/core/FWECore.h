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
 */

#include <functional>
#include <memory>
#include <string>

namespace fwe {

// Forward declarations
class ModelSubMain;
class MaterialSubMain;
class EnvironmentSubMain;
class SimulateSubMain;
class SceneGraph;
class IPCBroker;
class PanicSystem;
class StateManager;

// ─────────────────────────────────────────────
// Sub-main lifecycle
// ─────────────────────────────────────────────

enum class SubMain {
    Model,
    Material,
    Environment,
    Simulate
};

enum class SubMainState {
    Dormant,    // subprocess not running, no memory held
    Active,     // running, context loaded
    KeepAlive   // running in background while another sub-main is active
};

// ─────────────────────────────────────────────
// FWE Core
// ─────────────────────────────────────────────

class FWECore {
public:
    FWECore();
    ~FWECore();

    // Lifecycle
    bool initialize();
    void shutdown();

    // Sub-main management
    bool activateSubMain(SubMain target);
    bool setKeepAlive(SubMain target, bool enabled);
    SubMainState getSubMainState(SubMain target) const;

    // Context serialization (cold path IPC)
    bool serializeContext(SubMain source);
    bool restoreContext(SubMain target);

    // Panic function — kills all active compute immediately
    void triggerPanic(const std::string& reason = "");
    void setPanicThreshold(float tempCelsius, float gpuLoadPercent);

    // Cross-tab dependency tracking
    void notifyModelChanged();
    bool isSimulateStale() const;

    // Scene graph access
    SceneGraph& sceneGraph();
    const SceneGraph& sceneGraph() const;

    // Callbacks
    using PanicCallback = std::function<void(const std::string&)>;
    void onPanic(PanicCallback cb);

    using DependencyCallback = std::function<void()>;
    void onModelChangedWhileSimulateActive(DependencyCallback cb);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace fwe
