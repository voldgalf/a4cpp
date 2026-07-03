//
// Created by brick on 6/29/2026.
//

#include "../include/WorkFlowManager.h"

WorkflowManager::WorkflowManager() {
    current_node = 0;
}

void WorkflowManager::add_logic_nodes_sync(std::vector<std::unique_ptr<Logic_Node> > nodes_sync) {
    // Synchronously adds each Node

    for (std::unique_ptr<Logic_Node> &node: nodes_sync) {
        auto workflow_node = std::make_unique<Workflow_Node>();

        workflow_node->type = NODE;

        workflow_node->node = std::move(node);

        workflow_nodes.emplace_back(std::move(workflow_node));
    }
}

void WorkflowManager::add_logic_nodes_async(std::vector<std::unique_ptr<Logic_Node> > nodes_async) {
    // Asynchronously adds each Node

    auto workflow_node = std::make_unique<Workflow_Node>();

    workflow_node->type = PARALLEL_NODES;

    workflow_node->parallel_nodes = std::move(nodes_async);
    workflow_nodes.emplace_back(std::move(workflow_node));
}

void WorkflowManager::run_workflow_node(const std::unique_ptr<Workflow_Node> &workflow_node) {
    switch (workflow_node->type) {
        case NODE: {
            workflow_node->node->run(state);
            break;
        }
        case PARALLEL_NODES: {
            std::vector<std::future<void> > futures;

            for (std::unique_ptr<Logic_Node> &node: workflow_node->parallel_nodes) {
                futures.push_back(std::async(std::launch::async,
                                             [&node, this] { node->run(state); }
                ));
            }

            for (std::future<void> &future: futures) {
                future.get();
            }

            break;
        }
        case LOGIC_STATEMENT: {
            break;
        }
    }
}

void WorkflowManager::run() {
    for (std::unique_ptr<Workflow_Node> &workflow_node: workflow_nodes) {
        run_workflow_node(workflow_node);
    }
}
