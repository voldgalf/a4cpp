//
// Created by brick on 6/29/2026.
//
#pragma once
#include <memory>
#include <Logic_Node.h>
#include <future>

/**
 * @brief Identifies how a Workflow_Node should be executed by WorkflowManager.
 */
enum WorkflowNode_Types {
    /** @brief A single Logic_Node, run synchronously via Workflow_Node::node. */
    NODE,
    /** @brief A group of Logic_Node instances, run concurrently via Workflow_Node::parallel_nodes. */
    PARALLEL_NODES,
    /** @brief Reserved for conditional/branching logic; not yet implemented (currently a no-op). */
    LOGIC_STATEMENT
};

/**
 * @brief A single step in a WorkflowManager's execution list.
 *
 * Depending on #type, a Workflow_Node holds either one Logic_Node to run
 * synchronously (#node) or a collection of Logic_Node instances to run
 * concurrently (#parallel_nodes). WorkflowManager constructs and owns these
 * via add_logic_nodes_sync() and add_logic_nodes_async(), and executes them
 * via WorkflowManager::run_workflow_node().
 */
class Workflow_Node {
public:
    /** @brief Determines which of #node or #parallel_nodes is used, and how. */
    WorkflowNode_Types type;
    /** @brief The Logic_Node to run when #type is NODE. Unused otherwise. */
    std::unique_ptr<Logic_Node> node;
    /** @brief The Logic_Node instances to run concurrently when #type is PARALLEL_NODES. Empty otherwise. */
    std::vector<std::unique_ptr<Logic_Node> > parallel_nodes = {};
};

/**
 * @brief Orchestrates the sequential execution of a series of Logic_Node steps.
 *
 * A WorkflowManager owns an ordered list of Workflow_Node entries. Each entry is
 * either a single Logic_Node run synchronously, or a group of Logic_Node instances
 * run concurrently (see add_logic_nodes_sync() and add_logic_nodes_async()). A single
 * nlohmann::json state object is threaded through every node's run() call, allowing
 * nodes to read and write shared workflow state as execution progresses.
 *
 * Nodes are appended in the order they should execute; run() walks that list from
 * start to finish.
 */
class WorkflowManager {
protected:
    /** @brief Ordered list of workflow steps to execute, in registration order. */
    std::vector<std::unique_ptr<Workflow_Node> > workflow_nodes;
    /** @brief Index of the workflow step currently (or next) being executed. */
    int current_node = 0;
    /** @brief Shared state passed by reference to every Logic_Node's run() call. */
    nlohmann::json state = {};

    /**
     * @brief Executes a single workflow step according to its Workflow_Node::type.
     *
     * For type NODE, runs the contained Logic_Node synchronously against #state.
     * For type PARALLEL_NODES, launches each contained Logic_Node asynchronously
     * (std::launch::async) and blocks until all of them complete. LOGIC_STATEMENT
     * is not yet implemented and is currently a no-op.
     *
     * @param workflow_node The workflow step to execute.
     */
    void run_workflow_node(const std::unique_ptr<Workflow_Node> &workflow_node);

public:
    /** @brief Constructs an empty WorkflowManager with no registered nodes. */
    WorkflowManager();

    /**
     * @brief Appends a set of Logic_Node instances that will each run synchronously.
     *
     * Each node in @p nodes_sync becomes its own Workflow_Node of type NODE and is
     * appended to the workflow in order, so they execute one after another on run().
     *
     * @param nodes_sync Logic_Node instances to add; ownership is transferred to the WorkflowManager.
     */
    void add_logic_nodes_sync(std::vector<std::unique_ptr<Logic_Node> > nodes_sync);

    /**
     * @brief Appends a set of Logic_Node instances that will run concurrently as a single step.
     *
     * All nodes in @p nodes_async are grouped into one Workflow_Node of type
     * PARALLEL_NODES. When that step is reached during run(), every node in the
     * group is launched on its own thread and the workflow waits for all of them
     * to finish before advancing.
     *
     * @param nodes_async Logic_Node instances to run in parallel; ownership is transferred to the WorkflowManager.
     */
    void add_logic_nodes_async(std::vector<std::unique_ptr<Logic_Node> > nodes_async);

    /**
     * @brief Executes every registered workflow step in order.
     *
     * Iterates #workflow_nodes from front to back, dispatching each to
     * run_workflow_node(). Synchronous steps run inline; parallel steps block
     * until all of their nodes complete before the next step begins.
     */
    void run();
};
