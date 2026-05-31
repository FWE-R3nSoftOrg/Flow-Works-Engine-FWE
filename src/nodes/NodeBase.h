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
 * Tier 2 — pre-bundled Tier 1 with sensible defaults. Same system, two entry points.
 */

#include <memory>
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
    std::string  name;
    ParamValue   value;
    std::string  unit;       // display unit, e.g. "Hz", "N·m", "°C"
    float        min = 0.f;
    float        max = 0.f;
    bool         hasRange = false;
};

// ─────────────────────────────────────────────
// Port — typed connection point on a node
// ─────────────────────────────────────────────

enum class PortType {
    Mechanical,
    Thermal,
    Electrical,
    Acoustic,
    Fluid,
    Optical,
    Magnetic,
    Control,
    Signal,
    Force,
    Torque,
    Any
};

enum class PortDirection { Input, Output };

struct NodePort {
    std::string   name;
    PortType      type;
    PortDirection direction;
    bool          required = false;
};

// ─────────────────────────────────────────────
// Base node
// ─────────────────────────────────────────────

enum class NodeTier { One, Two };

class NodeBase {
public:
    virtual ~NodeBase() = default;

    virtual std::string  nodeId()          const = 0;  // unique type ID, e.g. "fwe.acoustic.speaker"
    virtual std::string  displayName()     const = 0;
    virtual NodeTier     tier()            const = 0;
    virtual std::string  category()        const = 0;  // "Acoustic", "Mechanical", etc.

    // Ports
    virtual std::vector<NodePort> ports() const = 0;

    // Parameters
    virtual std::vector<NodeParam>            params()                          const = 0;
    virtual bool                              setParam(const std::string& name,
                                                       const ParamValue& value)       = 0;
    virtual std::optional<ParamValue>         getParam(const std::string& name) const = 0;

    // Backend lifecycle — called by FWE core when node is placed/removed
    virtual bool onPlaced()  = 0;   // spin up backend
    virtual void onRemoved() = 0;   // release backend

    // Simulation tick — called by active sim engine each step
    virtual void tick(double deltaTime) {}
};

// ─────────────────────────────────────────────
// Node registry — all available node types
// ─────────────────────────────────────────────

class NodeRegistry {
public:
    static NodeRegistry& instance();

    void registerNode(std::unique_ptr<NodeBase> prototype);
    std::unique_ptr<NodeBase> create(const std::string& nodeId) const;
    std::vector<std::string>  allNodeIds() const;
    std::vector<std::string>  nodeIdsForCategory(const std::string& category) const;

private:
    NodeRegistry() = default;
    std::unordered_map<std::string, std::unique_ptr<NodeBase>> m_registry;
};

} // namespace fwe::nodes
