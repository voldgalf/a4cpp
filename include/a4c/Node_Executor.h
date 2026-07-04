//
// Created by mike on 7/3/26.
//

#pragma once

#include <functional>
#include <vector>

#include "../nlohmann/json.hpp"

enum Node_Type {
    SYNC,
    ASYNC,
};

struct Node_Prototype {
    Node_Type type;
    std::function<void(nlohmann::json &)> sync_logic;
    std::vector<std::function<void(nlohmann::json &)> > async_logic;
};

class Node_Executor {
protected:
    std::vector<Node_Prototype> nodes;

public:
    nlohmann::json state;

    Node_Executor() = default;

    bool create_async_node(std::vector<std::function<void(nlohmann::json &)> > async_nodes);

    bool create_sync_node(std::function<void(nlohmann::json &)> sync_node);

    bool run();
};
