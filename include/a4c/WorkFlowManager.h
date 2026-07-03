//
// Created by brick on 6/29/2026.
//
#pragma once
#include <memory>
#include "Logic_Node.h"
#include <future>
#include <functional>
#include <string>

enum WorkflowNode_Types {
    NODE,
    PARALLEL_NODES,
    LOGIC_STATEMENT
};

class Workflow_Node {
public:
    WorkflowNode_Types type;
    std::unique_ptr<Logic_Node> node;
    std::vector<std::unique_ptr<Logic_Node> > parallel_nodes = {};
};

class WorkflowManager {
protected:
    std::vector<std::unique_ptr<Workflow_Node> > workflow_nodes;
    int current_node = 0;
    nlohmann::json state = {};

    void run_workflow_node(const std::unique_ptr<Workflow_Node> &workflow_node);

public:
    WorkflowManager();

    void add_logic_nodes_sync(std::vector<std::unique_ptr<Logic_Node> > nodes_sync);

    void add_logic_nodes_async(std::vector<std::unique_ptr<Logic_Node> > nodes_async);

    void run();
};
