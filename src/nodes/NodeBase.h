#pragma once

/**
 * FWE Node System
 *
 * Universal — same nodes work in Environment and Simulate tabs.
 * Context-agnostic. All nodes composable.
 *
 * Node-as-initializer: dropping a node spins up its backend.
 * Removing a node releases its backend. No nodes = nothing running.
 *
 * Tier 1 — individual nodes, granular control, assigned to specific geometry.
 *           Node respects geometry — shape affects physics behavior.
 * Tier 2 — pre-bundled Tier 1 with sensible defaults. Same system, two entry points.
 *
 * Measurement nodes — live simulation taps, not display widgets.
 *                     Attach to any node output to read real-time values.
 *                     Also serve as QC checkpoints with threshold alerts.
 */

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace fwe::nodes {

// ─────────────────────────────────────────────
// Node parameter value types
// ─────────────────────────────────────────────

using ParamValue = std::variant<
    bool,
    int,
    float,
    double,
    std::string
>;

struct NodeParam {
    std::string name;
    ParamValue  value;
    std::string unit;        // display unit, e.g. "Hz", "N·m", "°C"
    float       min      = 0.f;
    float       max      = 0.f;
    bool        hasRange = false;
    std::string description;
};

// ─────────────────────────────────────────────
// Port — typed connection point on a node
// ─────────────────────────────────────────────

enum class PortType {
    // Physics domains
    Mechanical,
    Thermal,
    Electrical,
    Acoustic,
    Fluid,
    Optical,
    Magnetic,
    // Motion / force
    Force,
    Torque,
    // Signal / control
    Control,
    Signal,
    // Embedded / electronic
    Digital,    // GPIO, PWM, logic-level
    DataBus,    // UART, SPI, I2C, CAN
    Firmware,   // firmware behavior node input
    // Lighting
    DMX,        // DMX512 channel assignment
    // Catch-all
    Any
};

enum class PortDirection { Input, Output, Bidirectional };

struct NodePort {
    std::string   name;
    PortType      type;
    PortDirection direction;
    bool          required = false;
    std::string   description;
};

// ─────────────────────────────────────────────
// Node tiers and categories
// ─────────────────────────────────────────────

enum class NodeTier { One, Two };

// All node categories — maps to which domain structs get built at runtime
enum class NodeCategory {
    Mechanical,     // RPM, torque, joint, spring-damper, rigid body
    Acoustic,       // speaker, microphone, absorber, diffuser, point source
    Fluid,          // inlet, outlet, wall, pressure boundary, turbulence
    Thermal,        // heat source, heat sink, convection boundary, insulation
    Electrical,     // voltage source, ground, resistor, capacitor
    Magnetic,       // permanent magnet, coil, flux boundary
    Lighting,       // point light, area light, IES profile, sky dome, GDTF fixture
    Control,        // PID controller, lookup table, MIDI/OSC/timecode, automation
    Measurement,    // probe, sensor, data logger, FFT analyzer — live sim taps
    Embedded,       // microcontroller, ECU, drone FC, generic PCB
    Propulsion,     // combustion, turbine, marine drive — v0.2.0
    Visualization,  // overlay nodes, heatmap, particle system
    Environment,    // atmospheric, gravity, boundary condition nodes
    Custom          // user-defined scripted nodes
};

// ─────────────────────────────────────────────
// Measurement node — threshold and logging config
// ─────────────────────────────────────────────

struct MeasurementConfig {
    bool        loggingEnabled   = false;  // record over sim time → CSV export
    bool        graphOverlay     = true;   // live mini chart on the node
    bool        thresholdAlert   = false;  // flag when value goes out of range
    double      thresholdMin     = 0.0;
    double      thresholdMax     = 0.0;
    std::string displayUnit;               // unit conversion for display only — doesn't affect sim
};

// ─────────────────────────────────────────────
// Base node
// ─────────────────────────────────────────────

class NodeBase {
public:
    virtual ~NodeBase() = default;

    // Identity
    virtual std::string  nodeId()      const = 0;  // reverse-DNS style, e.g. "fwe.acoustic.speaker"
    virtual std::string  displayName() const = 0;
    virtual NodeTier     tier()        const = 0;
    virtual NodeCategory category()    const = 0;
    virtual std::string  backend()     const = 0;  // "openal" | "openfoam" | "calculix" | "none" etc.

    // Ports
    virtual std::vector<NodePort> ports() const = 0;

    // Parameters
    virtual std::vector<NodeParam>     params()                                    const = 0;
    virtual bool                       setParam(const std::string& name,
                                                const ParamValue& value)                 = 0;
    virtual std::optional<ParamValue>  getParam(const std::string& name)           const = 0;

    // Backend lifecycle — called by FWE core when node is placed/removed
    virtual bool onPlaced()  = 0;   // spin up backend
    virtual void onRemoved() = 0;   // release backend

    // Simulation tick — called by active sim engine each step
    virtual void tick(double deltaTime) {}

    // Measurement interface — optional, override in measurement nodes
    virtual bool                      isMeasurementNode()    const { return false; }
    virtual double                    currentValue()         const { return 0.0; }
    virtual MeasurementConfig&        measurementConfig()          { return m_measurementConfig; }
    virtual const MeasurementConfig&  measurementConfig()    const { return m_measurementConfig; }

    // Node cascade panic — called by FWE core when an upstream dependency fails
    // Default: propagate failure to all output ports
    virtual void onUpstreamFailure(const std::string& failedNodeId) {}

    // Failure condition check — called each tick
    // Return a non-empty string to signal failure (becomes the cascade reason)
    virtual std::optional<std::string> checkFailureCondition() const { return std::nullopt; }

protected:
    MeasurementConfig m_measurementConfig;
};

// ─────────────────────────────────────────────
// Node registry — all available node types
// ─────────────────────────────────────────────

class NodeRegistry {
public:
    static NodeRegistry& instance();

    void registerNode(std::unique_ptr<NodeBase> prototype);
    std::unique_ptr<NodeBase> create(const std::string& nodeId) const;

    std::vector<std::string> allNodeIds()                              const;
    std::vector<std::string> nodeIdsForCategory(NodeCategory category) const;
    std::vector<std::string> nodeIdsForTier(NodeTier tier)             const;

private:
    NodeRegistry() = default;
    std::unordered_map<std::string, std::unique_ptr<NodeBase>> m_registry;
};

} // namespace fwe::nodes
