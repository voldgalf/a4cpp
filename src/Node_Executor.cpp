//
// Created by mike on 7/3/26.
//
#include "../include/a4c/Node_Executor.h"


bool Node_Executor::create_sync_node(const lambda_node &sync_node) {
    const Node new_node{.type = SYNC, .sync_logic = sync_node};

    nodes.push_back(new_node);

    return true;
}

bool Node_Executor::create_async_node(const std::vector<lambda_node> &async_nodes) {
    const Node new_node{.type = ASYNC, .async_logic = async_nodes};
    nodes.push_back(new_node);

    return true;
}

bool Node_Executor::run() {
    for (Node &node: nodes) {
        switch (node.type) {
            case ASYNC: {
                std::vector<std::future<nlohmann::json> > logic_futures;

                for (std::function logic: node.async_logic) {
                    nlohmann::json state_clone = state;

                    logic_futures.push_back(std::async(std::launch::async, logic, state_clone));
                }

                for (std::future<nlohmann::json> &future: logic_futures) {
                    nlohmann::json state_modified = future.get();

                    state.merge_patch(state_modified);
                }

                break;
            }
            case SYNC: {
                nlohmann::json state_clone = state;

                state.merge_patch(state_clone);

                break;
            }
        }
    }

    return true;
}

nlohmann::json Node_Executor::get_state() const {
    return state;
}
