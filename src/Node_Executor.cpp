//
// Created by mike on 7/3/26.
//
#include "../include/a4c/Node_Executor.h"


bool node_executor::create_sync_node(const lambda_node &sync_node) {
    const node new_node{.type = SYNC, .sync_logic = sync_node};

    nodes_.push_back(new_node);

    return true;
}

bool node_executor::create_async_node(const std::vector<lambda_node> &async_nodes) {
    const node new_node{.type = ASYNC, .async_logic = async_nodes};
    nodes_.push_back(new_node);

    return true;
}

bool node_executor::run() {
    for (node &node: nodes_) {
        switch (node.type) {
            case ASYNC: {
                std::vector<std::future<nlohmann::json> > logic_futures;

                for (const lambda_node& logic: node.async_logic) {
                    nlohmann::json state_clone = state_;

                    logic_futures.push_back(std::async(std::launch::async, logic, state_clone));
                }

                for (std::future<nlohmann::json> &future: logic_futures) {
                    nlohmann::json state_modified = future.get();

                    state_.merge_patch(state_modified);
                }

                break;
            }
            case SYNC: {
                nlohmann::json state_clone = state_;

                state_.merge_patch(state_clone);

                break;
            }
        }
    }

    return true;
}

nlohmann::json node_executor::get_state() const {
    return state_;
}
