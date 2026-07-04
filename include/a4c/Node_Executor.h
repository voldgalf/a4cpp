//
// Created by mike on 7/3/26.
//

#pragma once

#include <functional>
#include <vector>
#include <future>

#include "../nlohmann/json.hpp"

using lambda_node = std::function<nlohmann::json(nlohmann::json state)>;

enum Node_Type {
    SYNC,
    ASYNC,
};

struct Node {
    Node_Type type;
    lambda_node sync_logic;
    std::vector<lambda_node> async_logic;
};

class Node_Executor {
protected:
    std::vector<Node> nodes;
    nlohmann::json state;

public:
    Node_Executor() = default;


    bool create_async_node(const std::vector<lambda_node> &async_nodes);

    template<typename T_input, typename T_output>
    bool map_async(std::function<T_output(T_input)> base_node, std::vector<T_input> input_vector) {
        std::vector<std::future<T_output> > logic_futures;


        for (T_input input_item: input_vector) {
            nlohmann::json state_copy = state;

            logic_futures.push_back(std::async(std::launch::async, base_node, input_item));
        }

        for (std::future<T_output> &future: logic_futures) {
            state.merge_patch(future.get());
        }

        return true;
    }

    bool create_sync_node(const lambda_node &sync_node);

    bool run();

    nlohmann::json get_state() const;
};
