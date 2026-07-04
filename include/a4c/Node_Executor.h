//
// Created by mike on 7/3/26.
//

#pragma once

#include <functional>
#include <vector>
#include <future>

#include "../nlohmann/json.hpp"

using lambda_node = std::function<nlohmann::json(nlohmann::json state)>;

enum node_type {
    SYNC,
    ASYNC,
};

struct node {
    node_type type;
    lambda_node sync_logic;
    std::vector<lambda_node> async_logic;
};

class node_executor {
protected:
    std::vector<node> nodes_;
    nlohmann::json state_;

public:
    node_executor() = default;


    bool create_async_node(const std::vector<lambda_node> &async_nodes);

    template<typename TInput, typename TOutput>
    bool map_async(std::function<TOutput(TInput)> base_node, std::vector<TInput> input_vector) {
        std::vector<std::future<TOutput> > logic_futures;


        for (TInput input_item: input_vector) {
            nlohmann::json state_copy = state_;

            logic_futures.push_back(std::async(std::launch::async, base_node, input_item));
        }

        for (std::future<TOutput> &future: logic_futures) {
            state_.merge_patch(future.get());
        }

        return true;
    }

    bool create_sync_node(const lambda_node &sync_node);

    bool run();

    nlohmann::json get_state() const;
};
