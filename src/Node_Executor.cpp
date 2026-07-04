//
// Created by mike on 7/3/26.
//
#include "../include/a4c/Node_Executor.h"


bool Node_Executor::create_sync_node(const std::function<void(nlohmann::json &)> &sync_node) {
    const Node new_node{.type = SYNC, .sync_logic = sync_node};

    nodes.push_back(new_node);

    return true;
}

bool Node_Executor::create_async_node(const std::vector<std::function<void(nlohmann::json &)> > &async_nodes) {
    const Node new_node{.type = ASYNC, .async_logic = async_nodes};
    nodes.push_back(new_node);

    return true;
}

bool Node_Executor::run() {
    for (Node &node: nodes) {
        switch (node.type) {
            case ASYNC: {
                std::vector<std::future<void> > logic_futures;

                for (std::function logic: node.async_logic) {
                    logic_futures.push_back(std::async(std::launch::async, logic, std::ref(state)));
                }

                for (std::future<void> &future: logic_futures) {
                    future.get();
                }

                break;
            }
            case SYNC: {
                node.sync_logic(std::ref(state));

                break;
            }
        }
    }

    return true;
}

nlohmann::json Node_Executor::get_state() const {
    return state;
}
