//
// Created by brick on 6/25/2026.
//
#pragma once
#include <nlohmann/json.hpp>


class Node {
public:
    virtual ~Node() = default;

    bool is_executed = false;

    virtual void run(nlohmann::json &state) = 0;
};
