//
// Created by brick on 6/25/2026.
//
#pragma once
#include <nlohmann/json.hpp>


/**
 * @brief Abstract base class for a single executable step within a workflow.
 *
 * A Logic_Node encapsulates one unit of work. WorkflowManager holds Logic_Node
 * instances (individually or in parallel groups) and invokes run() on each,
 * passing the workflow's shared state. Concrete subclasses implement run() to
 * read whatever inputs they need from @p state and write back any results.
 */
class Logic_Node {
public:
    /** @brief Virtual destructor to allow safe destruction through a base pointer. */
    virtual ~Logic_Node() = default;

    /**
     * @brief Executes this node's logic against the shared workflow state.
     *
     * Called by WorkflowManager as it steps through the workflow. When run as
     * part of a parallel group, this may be invoked concurrently with other
     * nodes' run() calls on separate threads, all sharing the same @p state.
     *
     * @param state Shared JSON state threaded through the entire workflow;
     *              implementations read their inputs from and write their
     *              outputs to this object.
     */
    virtual void run(nlohmann::json &state) = 0;
};
